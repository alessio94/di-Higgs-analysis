#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import collections
import logging
import math
import os

from ROOT import gROOT, gStyle
from ROOT import TCanvas, TPad, TH1F, TLatex, TLegend, TLine, TArrow
from ROOT import kBlack, kRed, kGreen, kBlue
from CAFbbll.atlas import set_atlas_style, draw_atlas_label, SIMWIP

PlotOptions = collections.namedtuple("PlotOptions", "line_color fill_style title")

PLOT_OPTIONS = dict()
PLOT_OPTIONS["signal"] = PlotOptions(title="Signal x10^{5}", line_color=kRed, fill_style=3005)
PLOT_OPTIONS["background"] = PlotOptions(title="Background (stat. error)", line_color=kBlack, fill_style=3004)
PLOT_OPTIONS["sigleft"] = PlotOptions(title="lower cut", line_color=kGreen + 2, fill_style=0)
PLOT_OPTIONS["sigright"] = PlotOptions(title="upper cut", line_color=kBlue + 1, fill_style=0)


def asimov_sig(hist_sig, hist_bkg, bin_start, bin_end):
    s = 0
    b = 0
    sigma_b_squared = 0
    for i in xrange(bin_start, bin_end + 1):
        s += hist_sig.GetBinContent(i)
        b += hist_bkg.GetBinContent(i)
        sigma_b_squared += hist_bkg.GetBinError(i) ** 2

    if s > 0 and b > 0 and sigma_b_squared > 0:
        first = (s + b) * math.log((s + b) * (b + sigma_b_squared) / (b ** 2 + (s + b) * sigma_b_squared))
        second = b ** 2 / sigma_b_squared * math.log(1 + sigma_b_squared * s / (b * (b + sigma_b_squared)))
        if first > second:
            sig = math.sqrt(2 * (first - second))
        else:
            sig = 0
        simple_sig = s / math.sqrt(b + sigma_b_squared)
        # logging.debug("s=%.3f, b=%.3f, sigma_b_squared=%.3f, sigma=%.5f, simple_sigma=%.5f", s, b, sigma_b_squared, sig, simple_sig)
        return sig
    else:
        return 0


def style_histogram(hist, version, pad_ratio, mode="plot"):
    title_size = 0.12
    lable_size = 0.12
    tick_length = 0.04
    hist.SetMarkerStyle(0)
    hist.SetLineColor(PLOT_OPTIONS[version].line_color)
    hist.SetLineWidth(2)
    hist.GetXaxis().SetTickLength(tick_length)
    if mode == "plot":
        hist.GetXaxis().SetTitleSize(title_size * (1 - pad_ratio) / pad_ratio)
        hist.GetXaxis().SetLabelSize(0)
        hist.GetYaxis().SetTitleSize(title_size * (1 - pad_ratio) / pad_ratio)
        hist.GetYaxis().SetLabelSize(lable_size * (1 - pad_ratio) / pad_ratio)
        hist.GetYaxis().SetTickLength(tick_length)
    if mode == "error":
        hist.SetFillColor(PLOT_OPTIONS[version].line_color)
        hist.SetFillStyle(PLOT_OPTIONS[version].fill_style)
    if mode == "sig":
        hist.GetXaxis().SetTitleSize(title_size)
        hist.GetXaxis().SetLabelSize(lable_size)
        hist.GetYaxis().SetTitleSize(title_size)
        hist.GetYaxis().SetLabelSize(lable_size)
        hist.GetYaxis().SetNdivisions(505)
        hist.GetXaxis().SetTitleOffset(1.0)
        hist.GetXaxis().SetTickLength(tick_length / (1 - pad_ratio) * pad_ratio)
        hist.SetFillColor(PLOT_OPTIONS[version].line_color)
        hist.SetFillStyle(PLOT_OPTIONS[version].fill_style)


def get_hist_minmax(hists, use_error=True):
    max_vals = list()
    min_vals = list()
    for hist in hists:
        for i in range(1, hist.GetNbinsX() + 1):  # range goes from 1 to number of bins
            if use_error:
                max_vals.append(hist.GetBinContent(i) + hist.GetBinError(i))
                min_vals.append(hist.GetBinContent(i) - hist.GetBinError(i))
            else:
                max_vals.append(hist.GetBinContent(i))
                min_vals.append(hist.GetBinContent(i))
    return min(min_vals), max(max_vals)


def move_bin(hist, old_bin, new_bin):
    """Move content and error of one bin to another bin

    Arguments:
        hist: histogram with bins to move
        old_bin: id of old bin
        new_bin: id of new bin
    """
    # get bin contents and errors for new and old bin
    old_bin_content = hist.GetBinContent(old_bin)
    old_bin_error = hist.GetBinError(old_bin)
    new_bin_content = hist.GetBinContent(new_bin)
    new_bin_error = hist.GetBinError(new_bin)

    # move old content to new content and treat errors
    hist.SetBinContent(new_bin, new_bin_content + old_bin_content)
    hist.SetBinError(new_bin, (new_bin_error ** 2.0 + old_bin_error ** 2.0) ** 0.5)

    # delete old bin contents
    hist.SetBinContent(old_bin, 0)
    hist.SetBinError(old_bin, 0)


def channel_to_latex(channel):
    if channel == "mm":
        return "#mu#mu"
    elif channel == "em":
        return "e#mu"
    elif channel == "me":
        return "#mue"
    elif channel == "sf":
        return "ee + #mu#mu"
    elif channel == "df":
        return "e#mu + #mue"
    else:
        return channel


def optimize_cut(reader, cut, observable, channel, output_dir):
    logging.info("Optimizting cut on '%s' for channel '%s'", observable, channel)
    channel_path = channel
    if channel == "sf":
        channel_path = "[ee+mm]"
    elif channel == "df":
        channel_path = "[em+me]"
    elif channel == "ll":
        channel_path = "[ee+mm+em+me]"

    hist_sig = reader.getHistogram(
        "sig/{channel}/nonres".format(channel=channel_path), "{cut}/{observable}".format(cut=cut, observable=observable)
    )
    hist_bkg = reader.getHistogram(
        "bkg/{channel}".format(channel=channel_path), "{cut}/{observable}".format(cut=cut, observable=observable)
    )
    nbins = hist_sig.GetNbinsX()

    # include underflow/overflow
    move_bin(hist_sig, nbins + 1, nbins)  # overflow
    move_bin(hist_sig, 0, 1)  # underflow
    move_bin(hist_bkg, nbins + 1, nbins)  # overflow
    move_bin(hist_bkg, 0, 1)  # underflow

    hist_bkg_error = hist_bkg.Clone("bkg_error_{channel}".format(channel=channel))

    hist_sig_left = hist_sig.Clone("hist_sig_left_{channel}".format(channel=channel))
    hist_sig_right = hist_sig.Clone("hist_sig_right_{channel}".format(channel=channel))

    logging.info("  scanning from left")
    highest_sig_left = -1
    highest_cut_left = -1
    for i in range(1, nbins + 1):
        sig = asimov_sig(hist_sig, hist_bkg, i, nbins)
        hist_sig_left.SetBinContent(i, sig)
        if sig > highest_sig_left:
            highest_sig_left = sig
            highest_cut_left = hist_sig.GetBinLowEdge(i)
        logging.debug("    %s > %.2f: %.5f", observable, hist_sig.GetBinLowEdge(i), sig)

    logging.info("  scanning from right")
    highest_sig_right = -1
    highest_cut_right = -1
    for i in reversed(range(1, nbins + 1)):
        sig = asimov_sig(hist_sig, hist_bkg, 0, i)
        hist_sig_right.SetBinContent(i, sig)
        if sig > highest_sig_right:
            highest_sig_right = sig
            highest_cut_right = hist_sig.GetBinLowEdge(i)
        logging.debug("    %s < %.2f: %.5f", observable, hist_sig.GetBinLowEdge(i), sig)

    canvas = TCanvas("canvas_{channel}".format(channel=channel), "", 800, 600)

    pad_height_ratio = 0.7
    pad_upper = TPad("pad_upper_" + channel, "", 0, 1 - pad_height_ratio + 0.005, 1, 1)
    pad_upper.SetBottomMargin(0)  # join upper and lower pad
    pad_upper.Draw()
    pad_upper.cd()

    hist_sig.Scale(1e5)
    hist_sig.Draw("HIST")
    hist_bkg.Draw("HIST SAME")
    hist_bkg_error.Draw("E2 SAME")

    style_histogram(hist_sig, "signal", pad_height_ratio)
    style_histogram(hist_bkg, "background", pad_height_ratio)
    style_histogram(hist_bkg_error, "background", pad_height_ratio, mode="error")

    _, hist_max = get_hist_minmax([hist_sig, hist_bkg])
    absolute_hist_max = hist_max / 0.7
    hist_sig.SetMaximum(absolute_hist_max)
    hist_sig.SetMinimum(0)
    hist_sig.GetYaxis().SetTitle("Events")

    pad_upper.Update()

    # draw cut lines
    line_y_max = absolute_hist_max
    line_y_min = 0
    line_y_middle = (line_y_max - line_y_min) / 2.0
    arrow_width = (hist_sig.GetBinLowEdge(nbins) + hist_sig.GetBinWidth(nbins) - hist_sig.GetBinLowEdge(1)) / 8
    arrow_size = 0.03

    sig_line_left = TLine(highest_cut_left, line_y_min, highest_cut_left, line_y_max)
    sig_line_left.SetLineColor(PLOT_OPTIONS["sigleft"].line_color)
    sig_line_left.SetLineWidth(2)
    sig_line_left.Draw()

    sig_arrow_left = TArrow(
        highest_cut_left, line_y_middle, highest_cut_left + arrow_width, line_y_middle, arrow_size, ">"
    )
    sig_arrow_left.SetLineColor(PLOT_OPTIONS["sigleft"].line_color)
    sig_arrow_left.SetLineWidth(2)
    sig_arrow_left.Draw()

    sig_line_right = TLine(highest_cut_right, line_y_min, highest_cut_right, line_y_max)
    sig_line_right.SetLineColor(PLOT_OPTIONS["sigright"].line_color)
    sig_line_right.SetLineWidth(2)
    sig_line_right.Draw()

    sig_arrow_right = TArrow(
        highest_cut_right - arrow_width, line_y_middle, highest_cut_right, line_y_middle, arrow_size, "<"
    )
    sig_arrow_right.SetLineColor(PLOT_OPTIONS["sigright"].line_color)
    sig_arrow_right.SetLineWidth(2)
    sig_arrow_right.Draw()

    # draw atlas labe and legend

    draw_atlas_label(0.2, 0.875, SIMWIP)

    latex = TLatex()
    latex.SetNDC()
    latex.SetTextFont(42)
    latex.DrawLatex(0.2, 0.8, "#sqrt{s} = 13 TeV, #lower[-0.2]{#scale[0.6]{#int}} Ldt = 36.1 fb^{-1}")
    latex.DrawLatex(0.2, 0.725, "HH #rightarrow bb(#tau#tau #rightarrow " + channel_to_latex(channel) + ")")

    legend = TLegend(0.65, 0.75, 0.9, 0.9)
    legend.SetBorderSize(0)
    legend.SetTextFont(42)
    legend.SetTextSize(0.04)
    legend.SetFillColor(0)
    legend.AddEntry(hist_sig, PLOT_OPTIONS["signal"].title, "l")
    legend.AddEntry(hist_bkg_error, PLOT_OPTIONS["background"].title, "lf")
    legend.Draw()

    # draw significance plots
    canvas.cd()
    pad_lower = TPad("pad_lower_" + channel, "", 0, 0, 1, 1 - pad_height_ratio)
    pad_lower.SetTopMargin(0)
    pad_lower.SetBottomMargin(0.275)
    pad_lower.Draw()
    pad_lower.cd()

    _, hist_max = get_hist_minmax([hist_sig_left, hist_sig_right], use_error=False)
    hist_sig_left.SetMinimum(0)
    hist_sig_left.SetMaximum(hist_max / 0.7)

    hist_sig_left.Draw("HIST")
    hist_sig_right.Draw("HIST SAME")

    style_histogram(hist_sig_left, "sigleft", pad_height_ratio, mode="sig")
    style_histogram(hist_sig_right, "sigright", pad_height_ratio, mode="sig")

    legend_sig = TLegend(0.65, 0.8, 0.9, 0.9)
    legend_sig.SetBorderSize(0)
    legend_sig.SetTextFont(42)
    legend_sig.SetTextSize(0.04 / (1 - pad_height_ratio) * pad_height_ratio)
    legend_sig.SetFillColor(0)
    legend_sig.SetNColumns(2)
    legend_sig.AddEntry(hist_sig_left, PLOT_OPTIONS["sigleft"].title, "l")
    legend_sig.AddEntry(hist_sig_right, PLOT_OPTIONS["sigright"].title, "l")
    legend_sig.Draw()

    pad_lower.Update()

    # manually set y-axis title for sig plot
    canvas.cd()
    ratio_title = TLatex()
    ratio_title.SetNDC()
    ratio_title.SetTextFont(42)
    ratio_title.SetTextSize(0.035)
    ratio_title.SetTextAngle(90)
    ratio_title.DrawLatex(0.06, 0.08, "Asimov sig.")
    canvas.Update()

    canvas.SaveAs(os.path.join(output_dir, "{observable}_{channel}.pdf".format(observable=observable, channel=channel)))

    logging.info("")

    return (highest_sig_left, highest_cut_left, highest_sig_right, highest_cut_right)


def main(args):
    gROOT.ProcessLine("gErrorIgnoreLevel = 1001;")  # disalbe INFO messages
    gROOT.SetBatch(True)  # enable batch mode
    gStyle.SetOptStat(0)  # disable statistics box
    set_atlas_style()

    logging.info("Loading sample folder, this may take a while ...")
    samples = TQSampleFolder.loadSampleFolder(args.input_file + ":" + args.sample_folder)
    reader = TQSampleDataReader(samples)

    output_dir = "results/cut_optimization"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    channels = ["ee", "mm", "em", "me", "sf", "df", "ll"]

    cut_info = dict()
    for channel in channels:
        cut_info[channel] = optimize_cut(reader, args.cut, args.observable, channel, output_dir)

    logging.info("Summary for optimization of cut on '%s'", args.observable)
    for channel in channels:
        logging.info(
            "{channel}: low cut: {observable} > {left}, high cut: {observable} < {right}".format(
                channel=channel, observable=args.observable, left=cut_info[channel][1], right=cut_info[channel][3]
            )
        )


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Compare plots from HH and H analysis")
    PARSER.add_argument("input_file", metavar="INPUT_FILE", type=str, help="Input file")
    PARSER.add_argument("cut", metavar="CUT", type=str, help="Previous cut of the cut to optimize")
    PARSER.add_argument("observable", metavar="OBSERVABLE", type=str, help="Observable for cut optimization")
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
