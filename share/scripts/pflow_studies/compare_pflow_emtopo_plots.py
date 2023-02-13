#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import collections
import fnmatch
import logging
import os

from ROOT import gROOT, gStyle
from ROOT import TCanvas, TPad, TLatex, TLegend
from ROOT import kBlack, kRed, kGreen, kBlue, kGray
from CAFbbll.atlas import set_atlas_style, draw_atlas_label, WIP


ALGOS = ["EMTopo", "EMPFlow"]

PlotOptions = collections.namedtuple("PlotOptions", "line_color fill_style title")

plot_options = dict()
plot_options["EMTopo"] = PlotOptions(title="EMTopo", line_color=kBlue, fill_style=3004)
plot_options["EMPFlow"] = PlotOptions(title="EMPFlow", line_color=kRed, fill_style=3005)


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


def channel_to_latex(channel):
    if channel == "sf":
        return "ee+#mu#mu"
    elif channel == "df":
        return "e#mu + #mue"
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
        return r"Full~Run-2"
    elif campaign == "c16a":
        return "2015+2016"
    elif campaign == "c16d":
        return "2017"
    elif campaign == "c16e":
        return "2018"
    else:
        return campaign


def make_comp_plot(readers, campaign, channel, process_info, cut_name, histo_name, output_dir, norm=False):
    logging.info("Plotting histogram %s/%s for campaign %s and channel %s", cut_name, histo_name, campaign, channel)
    caf_campaign = str_to_caf_campaign(campaign)
    caf_channel = str_to_caf_channel(channel)
    process_name, process_title, process_path = process_info

    hists = dict()
    versions = list()
    for algo in ALGOS:
        versions.append(algo)
        path = process_path.format(channel=caf_channel, campaign=caf_campaign)
        hists[algo] = readers[algo].getHistogram(path, "{}/{}".format(cut_name, histo_name))
        if not hists[algo]:
            logging.error(
                "Error retrieving histogram '{}/{}' from path '{}' for '{}'".format(cut_name, histo_name, path, algo)
            )
            return

    plot_name = "{}-{}-{}".format(process_name, cut_name, histo_name)
    canvas = TCanvas("canvas_" + channel + "_" + plot_name, "", 800, 600)

    # Draw histograms

    pad_height_ratio = 0.7
    pad_upper = TPad("pad_upper_" + plot_name, "", 0, 1 - pad_height_ratio, 1, 1)
    pad_upper.SetBottomMargin(0)  # join upper and lower pad
    pad_upper.Draw()
    pad_upper.cd()

    is_first = True
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

    hist_min, hist_max = get_hist_minmax(hists.values())
    hists[versions[0]].SetMinimum(min(hist_min, 0))
    hists[versions[0]].SetMaximum(hist_max / 0.7)
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
        latex.DrawLatex(0.2, 0.8, "#sqrt{s} = 13 TeV, #lower[-0.2]{#scale[0.6]{#int}} Ldt = 139 fb^{-1}")
    latex.DrawLatex(0.2, 0.74, "HH #rightarrow bbll (" + channel_to_latex(channel) + ")")
    latex.DrawLatex(0.2, 0.68, "Process: " + process_title)

    legend = TLegend(0.75, 0.75, 0.92, 0.92)
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

    ratios = [("EMPFlow", "EMTopo")]
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
    if ratio_max_value > 3.2:
        ratio_max_value = 3.2
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
    ratio_title.DrawLatex(0.05, 0.08, "EMPFlow / EMTopo")
    canvas.Update()

    canvas.SaveAs(os.path.join(output_dir, "{0:s}-{1:s}-lin.pdf".format(channel, plot_name)))
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

    algo_filenames = dict()
    algo_filenames["EMTopo"] = "emtopo"
    algo_filenames["EMPFlow"] = "pflow"
    samples = dict()
    readers = dict()
    for algo in ALGOS:
        logging.info("Loading sample folder for %s, this may take a while ...", algo)
        file_path_template = "sampleFolders/analyzed/samples-analyzed-{algo_filename}.root"
        sample_folder = TQSampleFolder.loadSampleFolder(
            file_path_template.format(algo_filename=algo_filenames[algo]) + ":" + args.sample_folder
        )
        samples[algo] = sample_folder
        readers[algo] = TQSampleDataReader(sample_folder)

    output_dir_norm = "results/pflow_emtopo_norm"
    if not os.path.exists(output_dir_norm):
        os.makedirs(output_dir_norm)

    output_dir_scale = "results/pflow_emtopo_scale"
    if not os.path.exists(output_dir_scale):
        os.makedirs(output_dir_scale)

    channels = ["ll"]
    campaigns = ["run2"]

    histos = readers.values()[0].getListOfHistogramNames()
    histo_patterns = [
        "CutMllSR1/*Jet0*",
        "CutMllSR1/*Jet1*",
        "CutMllSR1/DiBJet*",
        "CutMllSR1/MET_*",
        "CutMllSR1/CentralJet*",
        "CutMllSR1/MHH",
    ]

    processes = list()
    processes.append(("signal", "Signal", "/sig/{channel}/{campaign}/nonres"))
    processes.append(("ttbar", "t#bar{t}", "/bkg/{channel}/{campaign}/top/ttbar"))
    processes.append(("data", "Data", "/data/{channel}/{campaign}"))
    # processes.append(("bbww", "bbWW", "/sig/{channel}/{campaign}/nonres/bbWW"))
    # processes.append(("bbtautau", "bb#tau#tau", "/sig/{channel}/{campaign}/nonres/bbtautau"))
    # processes.append(("bbzz", "bbZZ", "/sig/{channel}/{campaign}/nonres/bbZZ"))

    for campaign in campaigns:
        for channel in channels:
            for process_info in processes:
                for histo in histos:
                    if check_histo_match(str(histo.GetString()), histo_patterns):
                        cut_name, histogram_name = str(histo.GetString()).split("/")
                        make_comp_plot(
                            readers,
                            campaign,
                            channel,
                            process_info,
                            cut_name,
                            histogram_name,
                            output_dir_scale,
                            norm=False,
                        )
                        make_comp_plot(
                            readers,
                            campaign,
                            channel,
                            process_info,
                            cut_name,
                            histogram_name,
                            output_dir_norm,
                            norm=True,
                        )


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Compare plots for different electron WPs")
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
