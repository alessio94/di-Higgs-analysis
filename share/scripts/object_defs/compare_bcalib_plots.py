#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import collections
import fnmatch
import logging
import math
import os

from ROOT import gROOT, gStyle
from ROOT import TCanvas, TPad, TLatex, TLegend
from ROOT import kBlack, kRed, kGreen, kBlue, kGray, kCyan, kOrange
from CAFbbll.atlas import set_atlas_style, draw_atlas_label, WIP
from CAFbbll.bukin_fit import bukin_fit
from CAFbbll.latexfile import LaTeXFile


PlotOptions = collections.namedtuple("PlotOptions", "line_color fill_style line_style title")

plot_options = dict()
plot_options["default"] = PlotOptions(title="No additional corrections", line_color=kRed, fill_style=3004, line_style=1)
plot_options["muinjet_old"] = PlotOptions(
    title="#mu-in-jet (w/ e-loss corr.)", line_color=kGreen + 2, fill_style=3004, line_style=1
)
plot_options["muinjet_new"] = PlotOptions(
    title="#mu-in-jet (w/o e-loss corr.)", line_color=kBlue, fill_style=3005, line_style=1
)
plot_options["ptreco_old"] = PlotOptions(
    title="#mu-in-jet (w/ e-loss corr.) + pt reco (old)", line_color=kGreen + 2, fill_style=3004, line_style=2
)
plot_options["ptreco_new"] = PlotOptions(
    title="#mu-in-jet (w/ e-loss corr.) + pt reco (new)", line_color=kBlue, fill_style=3005, line_style=2
)


def style_histogram(hist, version, pad_ratio, mode="plot"):
    title_size = 0.12
    lable_size = 0.12
    tick_length = 0.03
    hist.SetMarkerStyle(0)
    hist.SetMarkerColor(plot_options[version].line_color)
    hist.SetLineColor(plot_options[version].line_color)
    hist.SetLineWidth(1)
    hist.GetXaxis().SetTickLength(tick_length)
    if mode == "plot":
        hist.GetXaxis().SetTitleSize(title_size * (1 - pad_ratio) / pad_ratio)
        hist.GetXaxis().SetLabelSize(0)
        hist.GetYaxis().SetTitleSize(title_size * (1 - pad_ratio) / pad_ratio)
        hist.GetYaxis().SetLabelSize(lable_size * (1 - pad_ratio) / pad_ratio)
        hist.GetYaxis().SetTickLength(tick_length)
        hist.SetLineStyle(plot_options[version].line_style)
    if mode == "error":
        hist.SetFillColor(plot_options[version].line_color)
        hist.SetFillStyle(plot_options[version].fill_style)
    if mode == "ratio":
        hist.GetXaxis().SetTitleSize(title_size)
        hist.GetXaxis().SetLabelSize(lable_size)
        hist.GetYaxis().SetTitleSize(title_size)
        hist.GetYaxis().SetLabelSize(lable_size)
        hist.GetYaxis().SetNdivisions(505)
        hist.GetXaxis().SetTitleOffset(1.0)
        hist.GetXaxis().SetTickLength(tick_length / (1 - pad_ratio) * pad_ratio)
        hist.SetLineStyle(plot_options[version].line_style)


def get_hist_minmax(hists, use_error=True):
    max_vals = list()
    min_vals = list()
    for hist in hists:
        for i in range(1, hist.GetNbinsX() + 1):  # range goes from 1 to n_bins
            if hist.GetBinContent(i) == 0 and hist.GetBinError(i) == 0:
                continue
            if use_error:
                max_vals.append(hist.GetBinContent(i) + hist.GetBinError(i))
                min_vals.append(hist.GetBinContent(i) - hist.GetBinError(i))
            else:
                max_vals.append(hist.GetBinContent(i))
                min_vals.append(hist.GetBinContent(i))
    return min(min_vals), max(max_vals)


def str_to_caf_channel(channel):
    if channel == "sf":
        return "[ee+mm]"
    elif channel == "df":
        return "[em+me]"
    elif channel == "ll":
        return "[ee+mm+em+me]"
    else:
        return channel


def channel_to_rootlatex(channel):
    if channel == "sf":
        return "ee+#mu#mu"
    elif channel == "df":
        return "e#mu+#mue"
    elif channel == "em":
        return "e#mu"
    elif channel == "me":
        return "#mue"
    elif channel == "ll":
        return "ee+#mu#mu+e#mu+#mue"
    else:
        return channel


def channel_to_latex(channel):
    if channel == "sf":
        return r"ee+\mu\mu"
    elif channel == "df":
        return r"e\mu{}+\mu{}e"
    elif channel == "em":
        return r"e\mu"
    elif channel == "me":
        return r"\mu{}e"
    elif channel == "ll":
        return r"ee+\mu\mu+e\mu{}+\mu{}e"
    else:
        return channel


def str_to_caf_campaign(campaign):
    if campaign == "run2":
        return "[c16a+c16d+c16e]"
    else:
        return campaign


def campaign_to_latex(campaign):
    if campaign == "run2":
        return r"Full~Run-2"
    elif campaign == "c16a":
        return "2015+2016"
    elif campaign == "c16d":
        return "2017"
    elif campaign == "c16e":
        return "2018"
    else:
        return campaign


def make_comp_plot(
    reader,
    campaign,
    channel,
    region,
    process_info,
    modes,
    histo_name,
    output_dir,
    norm=False,
    yields=None,
    yields_mu=None,
    yields_sigma=None,
):
    logging.info(
        "Plotting histogram %s for campaign %s and channel %s and process %s and region %s",
        histo_name,
        campaign,
        channel,
        process_info[1],
        region,
    )
    caf_campaign = str_to_caf_campaign(campaign)
    caf_channel = str_to_caf_channel(channel)
    process_name, process_title, process_path = process_info
    process_path_formatted = process_path.format(campaign=caf_campaign, channel=caf_channel)

    hists = dict()
    versions = list()
    for version, cut, replacement in modes:
        versions.append(version)
        histo_name_final = "{}/{}".format(cut.replace("SR1", region), histo_name.replace("BJet", replacement))
        hists[version] = reader.getHistogram(process_path_formatted, histo_name_final)
        if not hists[version]:
            logging.error("Error retrieving histogram '%s' from path '%s'", histo_name_final, process_path_formatted)
            return

    plot_name = "{}-{}-{}".format(region, process_name, histo_name)
    canvas = TCanvas("canvas_" + channel + "_" + plot_name, "", 800, 600)

    # Draw histograms

    pad_height_ratio = 0.7
    pad_upper = TPad("pad_upper_" + plot_name, "", 0, 1 - pad_height_ratio, 1, 1)
    pad_upper.SetBottomMargin(0)  # join upper and lower pad
    pad_upper.Draw()
    pad_upper.cd()

    is_first = True
    fit_results = dict()
    for version in versions:
        logging.debug("Plotting histogram %s for campaign %s and channel %s", version, campaign, channel)
        current_hist = hists[version]

        if is_first:
            if norm:
                current_hist = current_hist.DrawNormalized("HIST")
            else:
                current_hist = current_hist.DrawCopy("HIST")
            is_first = False
        else:
            if norm:
                current_hist = current_hist.DrawNormalized("HIST SAME")
            else:
                current_hist = current_hist.DrawCopy("HIST SAME")

        # draw error band
        current_hist_error = current_hist.DrawCopy("E2 SAME")

        style_histogram(current_hist, version, pad_height_ratio)
        style_histogram(current_hist_error, version, pad_height_ratio, "error")

        hists[version] = current_hist

        # apply Bukin fit to m_bb
        if "DiBJetMass" in histo_name:
            logging.info("Applying bukin fit for version %s", version)
            fit_results[version] = bukin_fit(current_hist)

    hist_min, hist_max = get_hist_minmax(hists.values())
    hists[versions[0]].SetMinimum(min(hist_min, 0))
    hists[versions[0]].SetMaximum(hist_max / 0.65)
    if norm:
        hists[versions[0]].GetYaxis().SetTitle("(1/N) Events")
    else:
        hists[versions[0]].GetYaxis().SetTitle("Events")
    pad_upper.Update()

    # draw fit results and get yields in mu +/- sigma
    yields = dict()
    for version in versions:
        if version not in fit_results:
            continue
        fit_results[version][0].Draw("SAME")

        if yields_mu is None:
            yields_mu = dict()
        if yields_sigma is None:
            yields_sigma = dict()

        if version in yields_mu:
            mu = yields_mu[version]
        else:
            mu = fit_results[version][1]
            yields_mu[version] = mu

        if version in yields_sigma:
            sigma = yields_sigma[version]
        else:
            sigma = fit_results[version][3]
            yields_sigma[version] = sigma

        yields[version] = fit_results[version][1], fit_results[version][3], get_yield(hists[version], mu, sigma)

    # draw atlas label, sqrt(s), channel and legend
    draw_atlas_label(0.2, 0.875, WIP)

    latex = TLatex()
    latex.SetNDC()
    latex.SetTextFont(42)
    if norm:
        latex.DrawLatex(0.2, 0.8, "#sqrt{s} = 13 TeV")
    else:
        latex.DrawLatex(0.2, 0.8, "#sqrt{s} = 13 TeV, #lower[-0.2]{#scale[0.6]{#int}} Ldt = 139 fb^{-1}")
    latex.DrawLatex(0.2, 0.74, "HH #rightarrow bbll (" + channel_to_rootlatex(channel) + ")")
    latex.DrawLatex(0.2, 0.68, "2tag, " + region + ", " + process_title)

    if not fit_results:
        legend = TLegend(0.52, 0.68, 0.92, 0.92)
    else:
        legend = TLegend(0.52, 0.30, 0.92, 0.92)
    legend.SetBorderSize(0)
    legend.SetTextFont(42)
    legend.SetTextSize(0.04)
    legend.SetFillStyle(0)
    for version in versions:
        if version in hists and hists[version] is not None:
            legend.AddEntry(hists[version], plot_options[version].title, "l")
        if version in fit_results:
            _, mean, mean_error, sigma, sigma_error = fit_results[version]
            ratio_sigma_mean = sigma / mean
            legend.AddEntry(
                None, "#mu={:.0f} GeV, #sigma={:.0f}, #sigma/#mu={:.2f}".format(mean, sigma, ratio_sigma_mean), ""
            )
    legend.AddEntry(None, "Error bands: stat. only uncert.", "")
    legend.Draw()

    # draw ratio plot
    canvas.cd()
    pad_lower = TPad("pad_lower_" + plot_name, "", 0, 0, 1, 1 - pad_height_ratio)
    pad_lower.SetTopMargin(0)
    pad_lower.SetBottomMargin(0.275)
    pad_lower.Draw()
    pad_lower.cd()

    ratios = [
        ("muinjet_old", "default"),
        ("muinjet_new", "default"),
        ("ptreco_old", "default"),
        ("ptreco_new", "default"),
    ]
    # make nominal ratio plot
    ratio_nom_process = ratios[0][1]
    hist_ratio_nom = hists[ratio_nom_process].Clone()
    ratio_nom_deltamax = 0
    for i in range(1, hist_ratio_nom.GetNbinsX() + 1):
        bin_content = hist_ratio_nom.GetBinContent(i)
        bin_error = hist_ratio_nom.GetBinError(i)
        hist_ratio_nom.SetBinContent(i, 1)
        if not bin_content == 0:
            hist_ratio_nom.SetBinError(i, bin_error / bin_content)
            ratio_nom_deltamax = max(ratio_nom_deltamax, bin_error / bin_content)
        else:
            hist_ratio_nom.SetBinError(i, 0)
    style_histogram(hist_ratio_nom, ratio_nom_process, pad_height_ratio, mode="ratio")
    hist_ratio_nom.SetLineColor(kGray)
    hist_ratio_nom.SetFillColor(kGray)
    hist_ratio_nom.SetFillStyle(1001)
    hist_ratio_nom.SetMinimum(0)
    hist_ratio_nom.Draw("E2")

    # plot ratios
    ratio_plots = list()
    ratio_max_values = list()
    ratio_min_values = list()
    for gen_num, gen_nom in ratios:
        logging.debug("Plotting ratio %s/%s", gen_num, gen_nom)
        hist_num = hists[gen_num].Clone()
        hist_nom = hists[gen_nom]

        hist_num.Divide(hist_num, hist_nom)
        hist_ratio = hist_num
        ratio_min_value, ratio_max_value = get_hist_minmax([hist_ratio], use_error=False)
        ratio_min_values.append(ratio_min_value)
        ratio_max_values.append(ratio_max_value)
        hist_ratio.Draw("HIST SAME")

        style_histogram(hist_ratio, gen_num, pad_height_ratio, mode="ratio")
        ratio_plots.append(hist_ratio)

    ratio_min_value = min(ratio_min_values + [1 - ratio_nom_deltamax]) * 0.9
    ratio_max_value = max(ratio_max_values + [1 + ratio_nom_deltamax]) * 1.1
    if ratio_max_value > 1.5:
        ratio_max_value = 1.5
    if ratio_min_value < 0.5:
        ratio_min_value = 0.5
    hist_ratio_nom.SetMinimum(ratio_min_value)
    hist_ratio_nom.SetMaximum(ratio_max_value)

    pad_lower.Update()

    # somehow y axis title of ratio plot is not drawn ... just draw a tlatex instead
    canvas.cd()
    ratio_title = TLatex()
    ratio_title.SetNDC()
    ratio_title.SetTextFont(42)
    ratio_title.SetTextSize(0.035)
    ratio_title.SetTextAngle(90)
    ratio_title.DrawLatex(0.05, 0.08, "Variation / No correction")
    canvas.Update()

    canvas.SaveAs(os.path.join(output_dir, "{0:s}-{1:s}-lin.pdf".format(channel, plot_name)))
    canvas.IsA().Destructor(canvas)  # delete canvas from memory, otherwise there are sometimes seg-faults ...

    return yields


def get_yield(hist, mu, sigma):
    """Get yield from histogram in the range mu +/- sigma.

    In case the limits of the integral are inside a bin, then this bin is included in the integral.

    Arguments:
        hist (TH1): Histogram to retrieve the yield from.
        mu (float): Mean value, used to define the range.
        sigma (float): Standard deviation, used to define the range.

    Returns:
        int: Yield in the range mu +/- sigma.
    """
    xmin = mu - sigma
    xmax = mu + sigma
    offset = hist.GetBinLowEdge(0)
    bin_width = hist.GetBinWidth(0)
    bin_min = int(math.floor((xmin - offset) / bin_width))
    bin_max = int(math.ceil((xmax - offset) / bin_width))
    return hist.Integral(bin_min, bin_max)


def check_histo_match(hist_path, patterns, vetos):
    for veto in vetos:
        if fnmatch.fnmatch(hist_path, veto):
            return False
    for pattern in patterns:
        if fnmatch.fnmatch(hist_path, pattern):
            return True
    return False


def get_list_of_histograms(histos, cut, patterns, vetos):
    list_of_histograms = set()
    histo_patterns = [cut + "/" + pattern for pattern in patterns]
    for histo in histos:
        if check_histo_match(histo.GetString().Data(), histo_patterns, vetos):
            histo_name = histo.GetString().Data()
            histo_name = histo_name.replace(cut + "/", "")
            list_of_histograms.add(histo_name)
    return sorted(list(list_of_histograms))


def main(args):
    gROOT.ProcessLine("gErrorIgnoreLevel = 1001;")  # disalbe INFO messages
    gROOT.SetBatch(True)  # enable batch mode
    gStyle.SetOptStat(0)  # disable statistics box
    set_atlas_style()

    logging.info("Loading sample folder, this may take a while ...")
    samples = TQSampleFolder.loadSampleFolder(args.input_file + ":" + args.sample_folder)
    reader = TQSampleDataReader(samples)

    output_dir_norm = "results/object_defs/bcalib_norm"
    if not os.path.exists(output_dir_norm):
        os.makedirs(output_dir_norm)

    output_dir_scale = "results/object_defs/bcalib_scale"
    if not os.path.exists(output_dir_scale):
        os.makedirs(output_dir_scale)

    output_dir_yields = "results/object_defs/bcalib_yields"
    if not os.path.exists(output_dir_yields):
        os.makedirs(output_dir_yields)

    channels = ["df", "sf", "ll"]
    campaigns = ["run2"]
    regions = ["SR1", "SR2", "TopCR"]

    modes = list()
    modes.append(("default", "Cut2TagMllSR1PtRecoNo", "BJet"))
    modes.append(("muinjet_old", "Cut2TagMllSR1MuInJetELoss", "BJetMuInJetELoss"))
    modes.append(("muinjet_new", "Cut2TagMllSR1MuInJet", "BJetMuInJet"))
    modes.append(("ptreco_old", "Cut2TagMllSR1PtRecoOld", "BJetPtRecoOld"))
    modes.append(("ptreco_new", "Cut2TagMllSR1PtRecoNew", "BJetPtRecoNew"))

    histos = reader.getListOfHistogramNames()
    histo_patterns = ["*BJet*"]
    histo_vetos = ["*Corrected*"]
    histos = get_list_of_histograms(histos, modes[0][1], histo_patterns, histo_vetos)

    processes = list()
    processes.append(("signal", "Signal", "/sig/{channel}/{campaign}/nonres"))
    processes.append(("bbww", "bbWW", "/sig/{channel}/{campaign}/nonres/bbWW"))
    processes.append(("bbtautau", "bb#tau#tau", "/sig/{channel}/{campaign}/nonres/bbtautau"))
    processes.append(("bbzz", "bbZZ", "/sig/{channel}/{campaign}/nonres/bbZZ"))
    processes.append(("ttbar", "t#bar{t}", "/bkg/{channel}/{campaign}/top/ttbar/nonallhad"))

    total_yields = dict()
    for campaign in campaigns:
        for channel in channels:
            for region in regions:
                if (channel == "df" or channel == "em" or channel == "me") and region == "SR2":
                    continue
                yields = dict()
                mus = dict()
                sigmas = dict()
                for process_info in processes:
                    for histo in histos:
                        current_yields = make_comp_plot(
                            reader,
                            campaign,
                            channel,
                            region,
                            process_info,
                            modes,
                            histo,
                            output_dir_scale,
                            False,
                            mus,
                            sigmas,
                        )
                        if (histo == "DiBJetMass" and process_info[0] == "ttbar") or (
                            histo == "DiBJetMass2" and process_info[0] != "ttbar"
                        ):
                            yields[process_info[0]] = current_yields
                        make_comp_plot(
                            reader, campaign, channel, region, process_info, modes, histo, output_dir_norm, norm=True
                        )
                total_yields[(campaign, channel, region)] = yields

    output_file_path = os.path.join(output_dir_yields, "bcalib.tex")
    with LaTeXFile.from_rel_path(output_file_path) as tex:
        tex.document_settings = ["12pt", "a4paper"]
        tex.write_header()
        tex.begin_document()

        for version_info, processes in total_yields.items():
            if version_info[2] == "TopCR":
                continue
            header, body = build_yield_table(processes)
            write_cutflow_table(tex, header, body, version_info)

        tex.end_document()
        tex.write_make_file()


def build_yield_table(yields):
    header = ["Version", r"$\mu_s$", r"$\sigma_s$", r"$\mu_b$", r"$\sigma_b$", "$s$", "$b$", "$s/b$"]
    body = list()

    versions = list()
    versions.append("default")
    versions.append("muinjet_old")
    versions.append("muinjet_new")
    versions.append("ptreco_old")
    versions.append("ptreco_new")

    for version in versions:
        row = list()
        row.append(plot_options[version].title.replace("#mu", r"$\mu$"))
        row.append("{:.2f}".format(yields["signal"][version][0]))  # mu
        row.append("{:.2f}".format(yields["signal"][version][1]))  # sigma
        row.append("{:.2f}".format(yields["ttbar"][version][0]))  # mu
        row.append("{:.2f}".format(yields["ttbar"][version][1]))  # sigma

        s = yields["signal"][version][2]  # yield
        b = yields["ttbar"][version][2]  # yield
        row.append("{:.2f}".format(s))
        row.append("{:.0f}".format(b))
        row.append("{:.7f}".format(s / b))

        body.append(row)

    return header, body


def write_cutflow_table(tex, header, body, process_info):
    n_cols = len(header)

    tex.writeline(r"\begin{table}[H]")
    tex.writeline(
        r"\caption{"
        + campaign_to_latex(process_info[0])
        + " -- $"
        + channel_to_latex(process_info[1])
        + "$ -- "
        + process_info[2]
        + "}"
    )
    tex.writeline(r"\begin{center}")
    tex.writeline(r"\begin{tabular}{@{}l" + "c" * n_cols + "@{}}")
    tex.writeline(r"\toprule")

    tex.writeline(" & ".join(header) + r" \\ \midrule")
    for row in body:
        if row == "|":
            tex.writeline(r"\midrule")
        elif isinstance(row, str):
            tex.writeline(r"\multicolumn{" + str(n_cols) + "}{c}{" + row + r"} \\")
        else:
            tex.writeline(" & ".join(row) + r" \\")

    tex.writeline(r"\bottomrule")
    tex.writeline(r"\end{tabular}")
    tex.writeline(r"\end{center}")
    tex.writeline(r"\end{table}")
    tex.writeline("\n\n\n\n")


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Compare plots for different b-jet correction methods")
    PARSER.add_argument("input_file", metavar="INPUT_FILE", type=str, help="Input file")
    PARSER.add_argument(
        "--sample-folder", dest="sample_folder", type=str, help="Name of sample folder", default="samples"
    )
    PARSER.add_argument("--debug", dest="debug", action="store_true", help="Print debug messages", default=False)
    ARGS = PARSER.parse_args()

    LOG_LEVEL = logging.DEBUG if ARGS.debug else logging.INFO
    logging.basicConfig(
        level=LOG_LEVEL, format="%(asctime)s - %(levelname)s - %(message)s", datefmt="%Y-%m-%d %H:%M:%S"
    )

    # only load QFramework after command line arguments have been parsed
    from QFramework import TQSampleFolder, TQSampleDataReader

    main(ARGS)
