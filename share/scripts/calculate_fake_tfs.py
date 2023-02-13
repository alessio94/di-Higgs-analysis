#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import array
import json
import logging
import os

from ROOT import gROOT, gStyle
from ROOT import TCanvas, TPad, TLatex
from ROOT import kBlue
from CAFbbll.atlas import set_atlas_style, draw_atlas_label, INT


def style_histogram(hist, color=kBlue):
    title_size = 0.04
    lable_size = 0.04
    tick_length = 0.03
    hist.SetMarkerStyle(0)
    hist.SetLineColor(color)
    hist.SetLineWidth(1)
    hist.GetXaxis().SetTickLength(tick_length)
    hist.GetYaxis().SetTickLength(tick_length)
    hist.GetXaxis().SetTitleSize(title_size)
    hist.GetXaxis().SetLabelSize(lable_size)
    hist.GetYaxis().SetTitleSize(title_size)
    hist.GetYaxis().SetLabelSize(lable_size)


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
        return "e#mu+#mue"
    elif channel == "em":
        return "e#mu"
    elif channel == "me":
        return "#mue"
    elif channel == "ll":
        return "ee+#mu#mu+e#mu+#mue"
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


def calculate_transfer_factors(reader, campaign, channel, cut, proc_os, proc_ss, histo_name, binning, output_dir):
    logging.info(
        "Getting transfer factor for for histogram %s for campaign %s, channel %s and cut %s",
        histo_name,
        campaign,
        channel,
        cut,
    )
    caf_campaign = str_to_caf_campaign(campaign)
    caf_channel = str_to_caf_channel(channel)

    process_versions = list()
    process_versions.append(("os", proc_os))
    process_versions.append(("ss", proc_ss))

    hists = dict()
    versions = list()
    for version, version_path in process_versions:
        process_path_formatted = version_path.format(campaign=caf_campaign, channel=caf_channel)
        logging.debug("Getting histogram %s/%s from path '%s'", cut, histo_name, process_path_formatted)
        hists[version] = reader.getHistogram(process_path_formatted, "{}/{}".format(cut, histo_name))
        if binning:
            hists[version] = hists[version].Rebin(len(binning) - 1, "", array.array("d", binning))
        versions.append(version)
        if not hists[version]:
            logging.error("Error retrieving histogram '%s/%s' from path '%s'", cut, histo_name, process_path_formatted)
            return

    plot_name = "{}-{}-{}".format(channel, cut, histo_name)

    transfer_factors = hists["os"].Clone("tfs_" + plot_name)
    transfer_factors.Divide(hists["ss"])

    # get info for json output files
    tf_bins = [transfer_factors.GetBinLowEdge(i) for i in range(1, transfer_factors.GetNbinsX() + 1)]
    last_bin = transfer_factors.GetNbinsX()
    tf_bins += [transfer_factors.GetBinLowEdge(last_bin) + transfer_factors.GetBinWidth(last_bin)]
    tf_values = [transfer_factors.GetBinContent(i) for i in range(1, transfer_factors.GetNbinsX() + 1)]
    tf_errors = [transfer_factors.GetBinError(i) for i in range(1, transfer_factors.GetNbinsX() + 1)]

    # Draw transfer factors

    canvas = TCanvas("canvas_" + plot_name, "", 800, 600)

    transfer_factors.Draw("HIST")
    transfer_factors_error = transfer_factors.DrawCopy("E0 SAME")

    style_histogram(transfer_factors)
    style_histogram(transfer_factors_error)

    transfer_factors.GetYaxis().SetTitle("f^{SS #rightarrow OS}")

    hist_min, hist_max = get_hist_minmax([transfer_factors])
    transfer_factors.SetMinimum(max(hist_min * 0.7, 0))
    transfer_factors.SetMaximum(hist_max / 0.7)
    canvas.Update()

    # draw atlas label, sqrt(s), channel and legend
    draw_atlas_label(0.2, 0.875, "    " + INT)

    latex = TLatex()
    latex.SetNDC()
    latex.SetTextFont(42)
    latex.DrawLatex(0.2, 0.8, "#sqrt{s} = 13 TeV, #lower[-0.2]{#scale[0.6]{#int}} Ldt = 139 fb^{-1}")
    latex.DrawLatex(0.2, 0.74, "HH #rightarrow bbll (" + channel_to_latex(channel) + ")")
    if cut == "Cut2TagMllSR1":
        cut_title = "SR1"
    elif cut == "Cut2TagMllSR1VBFVeto":
        cut_title = "SR1+VBFVeto"
    elif cut == "Cut2TagMbbSR2":
        cut_title = "SR2"
    elif cut == "Cut2TagMBLTTbarCR":
        cut_title = "TTbar CR"
    elif cut == "Cut2TagMBLWtCR":
        cut_title = "Wt CR"
    elif cut == "Cut2TagMbbZllCR":
        cut_title = "Zll CR"
    else:
        cut_title = cut
    latex.DrawLatex(0.2, 0.68, "Region: " + cut_title)

    canvas.SaveAs(os.path.join(output_dir, "{0:s}.pdf".format(plot_name)))
    canvas.IsA().Destructor(canvas)  # delete canvas from memory, otherwise there are sometimes seg-faults ...

    return tf_bins, tf_values, tf_errors


def write_tfs_to_json(tfs, channels, selections, dist_caf_expression, dist_output_name, output_dir, suffix=""):
    result_dict = dict()
    for channel in channels:
        channel_dict = dict()
        channel_dict["variable"] = dist_caf_expression
        if channel in selections:
            channel_dict["selection"] = selections[channel]
        channel_dict["binning"] = tfs[channel][0]
        channel_dict["values"] = tfs[channel][1]
        channel_dict["errors"] = tfs[channel][2]
        result_dict[channel] = channel_dict
    if suffix:
        output_file_name = "{}_{}.json".format(dist_output_name, suffix)
    else:
        output_file_name = "{}.json".format(dist_output_name)
    output_file_path = os.path.join(output_dir, output_file_name)
    with open(output_file_path, "w") as output_file:
        json.dump(result_dict, output_file, indent=4)


def main(args):
    gROOT.ProcessLine("gErrorIgnoreLevel = 1001;")  # disalbe INFO messages
    gROOT.SetBatch(True)  # enable batch mode
    gStyle.SetOptStat(0)  # disable statistics box
    set_atlas_style()

    samples = TQSampleFolder.loadLazySampleFolder(args.input_file + ":" + args.sample_folder)
    reader = TQSampleDataReader(samples)

    output_dir = "results/fake_transfer_factors"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    channels = ["ll", "ee", "mm", "em", "me"]
    selections = dict()
    selections["ee"] = "$(isEE)"
    selections["mm"] = "$(isMM)"
    selections["em"] = "$(isEM)"
    selections["me"] = "$(isME)"
    campaigns = ["run2"]

    cuts = ["Cut2TagMllSR1"]

    process_os_mc = "/bkg/{channel}/{campaign}/nonprompt"
    process_ss_mc = "/bkg/{channel}/{campaign}/ddFakes/mc/nonprompt"

    distributions = [
        ("Lepton0Pt", "$(Lepton0Pt)/1000.", "lep_0_pt", None),
        ("Lepton1Pt", "$(Lepton1Pt)/1000.", "lep_1_pt", [0, 10, 20, 30, 40, 60, 75, 100]),
        ("DiLeptonPt", "$(DiLeptonPt)/1000.", "pt_ll", [0, 10, 20, 30, 70, 110, 150, 200, 300]),
        ("NN_SR1_Signal_Rebin", "$(NN_SR1_NodeSignal)", "nn_sr1", None),
    ]

    for campaign in campaigns:
        for distribution, dist_caf_expression, dist_output_name, binning in distributions:
            for cut in cuts:
                results = dict()
                for channel in channels:
                    results[channel] = calculate_transfer_factors(
                        reader, campaign, channel, cut, process_os_mc, process_ss_mc, distribution, binning, output_dir
                    )
                write_tfs_to_json(results, ["ll"], selections, dist_caf_expression, dist_output_name, output_dir)
                write_tfs_to_json(
                    results,
                    ["ee", "mm", "em", "me"],
                    selections,
                    dist_caf_expression,
                    dist_output_name,
                    output_dir,
                    suffix="channelsplit",
                )


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Compare plots for different btagging WPs")
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
