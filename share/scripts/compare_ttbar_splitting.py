#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import collections
import fnmatch
import logging
import os

from ROOT import gROOT, gStyle
from ROOT import TCanvas, TPad, TLatex, TLegend
from ROOT import kBlack, kGray, kGreen, kBlue
from CAFbbll.atlas import set_atlas_style, draw_atlas_label, WIP

PlotOptions = collections.namedtuple("PlotOptions", "line_color fill_style title")

plot_options = dict()
plot_options["dilepton"] = PlotOptions(title="dilepton", line_color=kBlack, fill_style=3004)
plot_options["nonallhad"] = PlotOptions(title="nonallhad (2l, TTBarDecayCode)", line_color=kGreen + 2, fill_style=3004)
plot_options["nonallhad_MCTC"] = PlotOptions(
    title="nonallhad (2l, MCTruthClassifier)", line_color=kBlue, fill_style=3005
)


def style_histogram(hist, version, pad_ratio, mode="plot"):
    title_size = 0.12
    lable_size = 0.12
    tick_length = 0.03
    hist.SetMarkerStyle(0)
    hist.SetLineColor(plot_options[version].line_color)
    hist.SetLineWidth(1)
    hist.GetXaxis().SetTickLength(tick_length)
    if mode == "plot":
        hist.GetXaxis().SetTitleSize(title_size * (1 - pad_ratio) / pad_ratio)
        hist.GetXaxis().SetLabelSize(0)
        hist.GetYaxis().SetTitleSize(title_size * (1 - pad_ratio) / pad_ratio)
        hist.GetYaxis().SetLabelSize(lable_size * (1 - pad_ratio) / pad_ratio)
        hist.GetYaxis().SetTickLength(tick_length)
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


def get_hist_minmax(hists, use_error=True, count_zeros=True):
    max_vals = list()
    min_vals = list()
    for hist in hists:
        for i in range(1, hist.GetNbinsX() + 1):  # range goes from 1 to n_bins
            content = hist.GetBinContent(i)
            error = hist.GetBinError(i)
            if use_error:
                max_vals.append(content + error)
                min_vals.append(content - error)
            else:
                max_vals.append(content)
                if count_zeros or content > 0:
                    min_vals.append(content)
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


def channel_to_latex(channel):
    if channel == "ll":
        return "ee+#mu#mu+e#mu+#mue"
    elif channel == "sf":
        return "ee+#mu#mu"
    elif channel == "df":
        return "e#mu+#mue"
    elif channel == "em":
        return "e#mu"
    elif channel == "me":
        return "#mue"
    else:
        return channel


def str_to_caf_campaign(campaign):
    if campaign == "run2":
        return "[c16a+c16d+c16e]"
    else:
        return campaign


def campaign_to_latex(campaign):
    if campaign == "run2":
        return "Full Run 2"
    elif campaign == "c16a":
        return "mc16a"
    elif campaign == "c16d":
        return "mc16d"
    elif campaign == "c16e":
        return "mc16e"
    else:
        return campaign


def campaign_to_lumi(campaign):
    if campaign == "run2":
        return "139"
    elif campaign == "c16a":
        return "36.2"
    elif campaign == "c16d":
        return "44.3"
    elif campaign == "c16e":
        return "58.5"
    else:
        return "0.0"


def cut_to_region(cut_name):
    region = ""
    if "1Tag" in cut_name:
        region += "1 tag, "
    if "2Tag" in cut_name:
        region += "2 tag, "
    if "SR1" in cut_name:
        region += "SR1"
    elif "SR2" in cut_name:
        region += "SR2"
    elif "TopCR" in cut_name:
        region += "TopCR"
    else:
        region = cut_name

    return region


def make_comp_plot(reader, channel, campaign, cut_name, histo_name, output_dir, norm=False):
    logging.info("Plotting histogram %s/%s for channel %s", cut_name, histo_name, channel)
    caf_channel = str_to_caf_channel(channel)
    caf_campaign = str_to_caf_campaign(campaign)
    hists = dict()
    hists["dilepton"] = reader.getHistogram(
        "bkg/{}/{}/top/ttbar/dilepton".format(caf_channel, caf_campaign), "{}/{}".format(cut_name, histo_name)
    )
    hists["nonallhad"] = reader.getHistogram(
        "bkg/{}/{}/top/ttbar/nonallhad".format(caf_channel, caf_campaign), "{}/{}".format(cut_name, histo_name)
    )
    hists["nonallhad_MCTC"] = reader.getHistogram(
        "bkg/{}/{}/top/ttbar/nonallhad_MCTC".format(caf_channel, caf_campaign), "{}/{}".format(cut_name, histo_name)
    )
    versions = ["dilepton", "nonallhad", "nonallhad_MCTC"]

    if not hists["dilepton"]:
        logging.error(
            "Error retrieving histogram '{}/{}' from path 'bkg/{}/{}/top/ttbar/dilepton'".format(
                cut_name, histo_name, caf_channel, caf_campaign
            )
        )
        return

    if not hists["nonallhad"]:
        logging.error(
            "Error retrieving histogram '{}/{}' from path 'bkg/{}/{}/top/ttbar/nonallhad'".format(
                cut_name, histo_name, caf_channel, caf_campaign
            )
        )
        return

    if not hists["nonallhad_MCTC"]:
        logging.error(
            "Error retrieving histogram '{}/{}' from path 'bkg/{}/{}/top/ttbar/nonallhad_MCTC'".format(
                cut_name, histo_name, caf_channel, caf_campaign
            )
        )
        return

    plot_name = "{}-{}".format(cut_name, histo_name)
    canvas = TCanvas("canvas_" + channel + "_" + campaign + "_" + plot_name, "", 800, 600)

    # Draw histograms

    pad_height_ratio = 0.7
    pad_upper = TPad("pad_upper_" + plot_name, "", 0, 1 - pad_height_ratio, 1, 1)
    pad_upper.SetBottomMargin(0)  # join upper and lower pad
    pad_upper.Draw()
    pad_upper.cd()

    is_first = True
    for version in versions:
        logging.debug("Plotting histogram for %s", version)
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

    hist_min, hist_max = get_hist_minmax(hists.values())
    hists[versions[0]].SetMinimum(min(hist_min, 0))
    hists[versions[0]].SetMaximum(hist_max / 0.65)
    if norm:
        hists[versions[0]].GetYaxis().SetTitle("(1/N) Events")
    else:
        hists[versions[0]].GetYaxis().SetTitle("Events")
    pad_upper.Update()

    # draw atlas label, sqrt(s), channel and legend
    draw_atlas_label(0.2, 0.875, WIP)

    latex = TLatex()
    latex.SetNDC()
    latex.SetTextFont(42)
    if norm:
        latex.DrawLatex(0.2, 0.8, "#sqrt{s} = 13 TeV, " + campaign_to_latex(campaign))
    else:
        latex.DrawLatex(
            0.2,
            0.8,
            "#sqrt{s} = 13 TeV, #lower[-0.2]{#scale[0.6]{#int}} Ldt = " + campaign_to_lumi(campaign) + " fb^{-1}",
        )
    latex.DrawLatex(0.2, 0.74, "HH #rightarrow bbll (" + channel_to_latex(channel) + ")")
    latex.DrawLatex(0.2, 0.68, cut_to_region(cut_name))

    legend = TLegend(0.55, 0.80, 0.92, 0.92)
    legend.SetBorderSize(0)
    legend.SetTextFont(42)
    legend.SetTextSize(0.04)
    legend.SetFillColor(-1)
    for version in versions:
        legend.AddEntry(hists[version], plot_options[version].title, "l")
    legend.Draw()

    # draw ratio plot
    canvas.cd()
    pad_lower = TPad("pad_lower_" + plot_name, "", 0, 0, 1, 1 - pad_height_ratio)
    pad_lower.SetTopMargin(0)
    pad_lower.SetBottomMargin(0.275)
    pad_lower.Draw()
    pad_lower.cd()

    ratios = [("nonallhad", "dilepton"), ("nonallhad_MCTC", "dilepton")]
    # make nominal ratio plot
    ratio_nom_process = ratios[0][1]
    hist_ratio_nom = hists[ratio_nom_process].Clone()
    for i in range(1, hist_ratio_nom.GetNbinsX() + 1):
        bin_content = hist_ratio_nom.GetBinContent(i)
        bin_error = hist_ratio_nom.GetBinError(i)
        hist_ratio_nom.SetBinContent(i, 1)
        if not bin_content == 0:
            hist_ratio_nom.SetBinError(i, bin_error / bin_content)
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
        ratio_min_value, ratio_max_value = get_hist_minmax([hist_ratio], use_error=False, count_zeros=False)
        ratio_min_values.append(ratio_min_value)
        ratio_max_values.append(ratio_max_value)
        hist_ratio.Draw("HIST SAME")

        style_histogram(hist_ratio, gen_num, pad_height_ratio, mode="ratio")
        ratio_plots.append(hist_ratio)

    # ratio_min_values = [v for v in ratio_min_values if v != 0]
    ratio_min_value = min(ratio_min_values) * 0.9
    ratio_max_value = max(ratio_max_values) * 1.1
    if ratio_max_value > 3.2:
        ratio_max_value = 3.2
    if ratio_max_value < 1.1:
        ratio_max_value = 1.1
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
    ratio_title.DrawLatex(0.1, 0.08, "nonallhad / dilepton")
    canvas.Update()

    canvas.SaveAs(os.path.join(output_dir, "{0:s}-{1:s}-{2:s}-lin.pdf".format(channel, campaign, plot_name)))
    canvas.IsA().Destructor(canvas)  # delete canvas from memory, otherwise there are sometimes seg-faults ...


def check_histo_match(hist_path, patterns):
    for pattern in patterns:
        if fnmatch.fnmatch(hist_path, pattern):
            return True
    return False


def main(args):
    gROOT.ProcessLine("gErrorIgnoreLevel = 1001;")  # disalbe INFO messages
    gROOT.SetBatch(True)  # enable batch mode
    gStyle.SetOptStat(0)  # disable statistics box
    set_atlas_style()

    logging.info("Loading sample folder, this may take a while ...")
    samples = TQSampleFolder.loadSampleFolder(args.input_file + ":" + args.sample_folder)
    reader = TQSampleDataReader(samples)

    output_dir_norm = "results/compare_ttbar_splitting_norm"
    if not os.path.exists(output_dir_norm):
        os.makedirs(output_dir_norm)

    output_dir_scale = "results/compare_ttbar_splitting_scale"
    if not os.path.exists(output_dir_scale):
        os.makedirs(output_dir_scale)

    channels = ["df", "sf", "ll"]
    campaigns = ["c16a", "c16d", "c16e", "run2"]

    histos = reader.getListOfHistogramNames()
    histo_patterns = ["Cut2TagMllSR*/*"]

    for channel in channels:
        for campaign in campaigns:
            for histo in histos:
                if check_histo_match(str(histo.GetString()), histo_patterns):
                    cut_name, histogram_name = str(histo.GetString()).split("/")
                    if "SR2" in cut_name and channel in ["df", "ll"]:  # only plot sf for SR2
                        continue
                    make_comp_plot(reader, channel, campaign, cut_name, histogram_name, output_dir_scale, norm=False)
                    make_comp_plot(reader, channel, campaign, cut_name, histogram_name, output_dir_norm, norm=True)


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Compare different ttbar splitting methods")
    PARSER.add_argument("input_file", metavar="INPUT_FILE", type=str, help="Input TQSampleFolder")
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
