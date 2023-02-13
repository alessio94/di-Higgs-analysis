#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import collections
import logging
import os

from ROOT import gROOT, gStyle
from ROOT import TCanvas, TPad, TLatex, TLegend
from ROOT import kBlack, kRed, kGray
from CAFbbll.atlas import set_atlas_style, draw_atlas_label, WIP

PlotOptions = collections.namedtuple("PlotOptions", "line_color fill_style title")

plot_options = dict()
plot_options["hh"] = PlotOptions(title="HH #rightarrow bb#tau#tau", line_color=kRed, fill_style=3004)
plot_options["h"] = PlotOptions(title="H #rightarrow #tau#tau", line_color=kBlack, fill_style=3005)


def style_histogram(hist, version, pad_ratio, mode="plot"):
    title_size = 0.12
    lable_size = 0.12
    tick_length = 0.04
    hist.SetMarkerStyle(0)
    hist.SetLineColor(plot_options[version].line_color)
    hist.SetLineWidth(2)
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
            if use_error:
                max_vals.append(hist.GetBinContent(i) + hist.GetBinError(i))
                min_vals.append(hist.GetBinContent(i) - hist.GetBinError(i))
            else:
                max_vals.append(hist.GetBinContent(i))
                min_vals.append(hist.GetBinContent(i))
    return min(min_vals), max(max_vals)


def make_comp_plot(reader_hh, reader_h, channel, histo_name, histo_info, output_dir, norm=False):
    logging.info("Plotting histogram %s for channel %s", histo_name, channel)
    hists = dict()
    hists["hh"] = reader_hh.getHistogram("data/{channel}".format(channel=channel), histo_info[0])
    if not hists["hh"]:
        logging.error(
            "Error retrieving histogram '{cut_histo}' from path 'data/{channel}' for HH".format(
                cut_histo=hist_info[0], channel=channel
            )
        )
        return

    hists["h"] = reader_h.getHistogram("data/{channel}".format(channel=channel), histo_info[1])
    if not hists["h"]:
        logging.error(
            "Error retrieving histogram '{cut_histo}' from path 'data/{channel}' for H".format(
                cut_histo=hist_info[1], channel=channel
            )
        )
        return

    plot_name = histo_name
    canvas = TCanvas("canvas_" + channel + "_" + plot_name, "", 600, 600)

    # Draw histograms

    pad_height_ratio = 0.7
    pad_upper = TPad("pad_upper_" + plot_name, "", 0, 1 - pad_height_ratio, 1, 1)
    pad_upper.SetBottomMargin(0)  # join upper and lower pad
    pad_upper.Draw()
    pad_upper.cd()

    is_first = True
    for version in ["hh", "h"]:
        logging.debug("Plotting histogram of %s for channel %s", version, channel)
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

        # save normalized histogram in hists dict, so that the ratio plot is calculated from the normalized histograms
        hists[version] = current_hist

        # draw error band
        if norm:
            current_hist_error = current_hist.DrawNormalized("E2 SAME")
        else:
            current_hist_error = current_hist.DrawCopy("E2 SAME")

        style_histogram(current_hist, version, pad_height_ratio)
        style_histogram(current_hist_error, version, pad_height_ratio, "error")
    hist_min, hist_max = get_hist_minmax(hists.values())
    hists["hh"].SetMinimum(min(hist_min, 0))
    hists["hh"].SetMaximum(hist_max / 0.7)
    if norm:
        hists["hh"].GetYaxis().SetTitle("(1/N) Events")
    else:
        hists["hh"].GetYaxis().SetTitle("Events")
    pad_upper.Update()

    # draw atlas label, sqrt(s), channel and legend
    draw_atlas_label(0.2, 0.875, WIP)

    latex = TLatex()
    latex.SetNDC()
    latex.SetTextFont(42)
    if norm:
        latex.DrawLatex(0.2, 0.8, "#sqrt{s} = 13 TeV")
    else:
        latex.DrawLatex(0.2, 0.8, "#sqrt{s} = 13 TeV, #lower[-0.2]{#scale[0.6]{#int}} Ldt = 36.1 fb^{-1}")
    latex.DrawLatex(0.2, 0.725, "#tau#tau #rightarrow " + channel)

    legend = TLegend(0.72, 0.75, 0.9, 0.9)
    legend.SetBorderSize(0)
    legend.SetTextFont(42)
    legend.SetTextSize(0.04)
    legend.SetFillColor(-1)
    for version in ["hh", "h"]:
        legend.AddEntry(hists[version], plot_options[version].title, "l")
    legend.Draw()

    # draw ratio plot
    canvas.cd()
    pad_lower = TPad("pad_lower_" + plot_name, "", 0, 0, 1, 1 - pad_height_ratio)
    pad_lower.SetTopMargin(0)
    pad_lower.SetBottomMargin(0.275)
    pad_lower.Draw()
    pad_lower.cd()

    ratios = [("h", "hh")]
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
    ratio_nom_title = plot_options[ratio_nom_process].title
    hist_ratio_nom.GetYaxis().SetTitle("H / {}".format(ratio_nom_title))
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
        hist_ratio.GetYaxis().SetTitle("H")
        ratio_plots.append(hist_ratio)

    ratio_min_value = min(ratio_min_values) * 0.9
    ratio_max_value = max(ratio_max_values) * 1.1
    if ratio_max_value > 3.2:
        ratio_max_value = 3.2
    if ratio_min_value < 0:
        ratio_min_value = 0
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
    ratio_title.DrawLatex(0.1, 0.08, "H / HH")
    canvas.Update()

    canvas.SaveAs(os.path.join(output_dir, "{0:s}-{1:s}-lin.pdf".format(channel, plot_name)))
    canvas.IsA().Destructor(canvas)  # delete canvas from memory, otherwise there are sometimes seg-faults ...


def main(args):
    gROOT.ProcessLine("gErrorIgnoreLevel = 1001;")  # disalbe INFO messages
    gROOT.SetBatch(True)  # enable batch mode
    gStyle.SetOptStat(0)  # disable statistics box
    set_atlas_style()

    logging.info("Loading HH sample folder, this may take a while ...")
    samples_hh = TQSampleFolder.loadSampleFolder(args.input_file_hh + ":" + args.sample_folder)
    reader_hh = TQSampleDataReader(samples_hh)

    logging.info("Loading H sample folder, this may take a while ...")
    samples_h = TQSampleFolder.loadSampleFolder(args.input_file_h + ":" + args.sample_folder)
    reader_h = TQSampleDataReader(samples_h)

    output_dir_norm = "results/compare_HH_H_norm"
    if not os.path.exists(output_dir_norm):
        os.makedirs(output_dir_norm)

    output_dir_scale = "results/compare_HH_H_scale"
    if not os.path.exists(output_dir_scale):
        os.makedirs(output_dir_scale)

    histos = dict()
    histos["Lepton0Pt"] = ["CutCompHTT/Lepton0Pt", "CutCompHHBBTT/LeptonPt0"]
    histos["Lepton1Pt"] = ["CutCompHTT/Lepton1Pt", "CutCompHHBBTT/LeptonPt1"]

    channels = ["ee", "mm", "em", "me"]

    for channel in channels:
        for histo_name, histo_info in histos.items():
            make_comp_plot(reader_hh, reader_h, channel, histo_name, histo_info, output_dir_scale, norm=False)
            make_comp_plot(reader_hh, reader_h, channel, histo_name, histo_info, output_dir_norm, norm=True)


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Compare plots from HH and H analysis")
    PARSER.add_argument("input_file_hh", metavar="INPUT_FILE_HH", type=str, help="Input file for HH")
    PARSER.add_argument("input_file_h", metavar="INPUT_FILE_H", type=str, help="Input file for H")
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
