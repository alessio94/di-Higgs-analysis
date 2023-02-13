#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import logging
import os

from ROOT import gROOT, gStyle
from ROOT import TCanvas
from CAFbbll.atlas import set_atlas_style, draw_atlas_label, INT


def channel_to_latex(channel):
    if channel == "mm":
        return r"\mu\mu"
    elif channel == "em":
        return r"e\mu"
    elif channel == "me":
        return r"\mu{}e"
    elif channel == "sf":
        return r"ee + \mu\mu"
    elif channel == "df":
        return r"e\mu + \mu{}e"
    elif channel == "ll":
        return r"\ell\ell"
    else:
        return channel


def channel_to_sfpath(channel):
    if channel == "sf":
        return "[ee+mm]"
    elif channel == "df":
        return "[em+me]"
    elif channel == "ll":
        return "[ee+mm+em+me]"
    else:
        return channel


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


def campaign_to_sfpath(campaign):
    if campaign == "run2":
        return "[c16a+c16d+c16e]"
    else:
        return campaign


def style_histogram(hist):
    title_size = 0.04
    lable_size = 0.04
    tick_length = 0.03
    hist.GetXaxis().SetTickLength(tick_length)
    hist.GetYaxis().SetTickLength(tick_length)
    hist.GetXaxis().SetTitleSize(title_size)
    hist.GetXaxis().SetLabelSize(lable_size)
    hist.GetYaxis().SetTitleSize(title_size)
    hist.GetYaxis().SetLabelSize(lable_size)
    hist.GetZaxis().SetTitleSize(title_size)
    hist.GetZaxis().SetLabelSize(lable_size)


def make_2d_plot(reader, campaign, channel, cut, distribution, process_title, process_name, process_path, output_dir):
    campaign_caf = campaign_to_sfpath(campaign)
    channel_caf = channel_to_sfpath(channel)
    full_process_path = process_path.format(campaign=campaign_caf, channel=channel_caf)
    logging.info("Making plot %s/%s for %s", cut, distribution, full_process_path)
    hist = reader.getHistogram(full_process_path, "{}/{}".format(cut, distribution))

    plot_name = "{}-{}-{}-{}-{}".format(channel, campaign, cut, distribution, process_name)
    canvas = TCanvas("canvas_" + plot_name, "", 800, 800)
    canvas.SetRightMargin(0.2)
    canvas.SetLeftMargin(0.12)
    canvas.SetTopMargin(0.1)
    canvas.SetBottomMargin(0.18)
    hist_title = process_title
    if channel != "ll":
        hist_title += " - {} channel".format(channel)
    hist.SetTitle(hist_title)
    style_histogram(hist)
    draw_atlas_label(0.16, 0.83, "    " + INT)
    for log_version in ["lin", "log"]:
        if log_version == "log":
            canvas.SetLogz(1)
            hist.SetMinimum(0.1)
        hist.Draw("COLZ")
        canvas.Update()
        canvas.SaveAs(os.path.join(output_dir, "{}-{}.pdf".format(plot_name, log_version)))
    canvas.IsA().Destructor(canvas)  # delete canvas from memory, otherwise there are sometimes seg-faults ...


def main(args):
    gROOT.ProcessLine("gErrorIgnoreLevel = 1001;")  # disalbe INFO messages
    gROOT.SetBatch(True)  # enable batch mode
    set_atlas_style()
    gStyle.SetOptTitle()  # enable printing of canvas title

    logging.info("Loading sample folder, this may take a while ...")
    samples = TQSampleFolder.loadSampleFolder(args.input_file + ":" + args.sample_folder)
    reader = TQSampleDataReader(samples)

    channels = ["ll"]
    # channels = ["ll", "ee", "mm", "em", "me"]
    campaigns = ["run2"]

    processes = list()
    processes.append((r"Signal (ggF + VBF)", "signal", "/sig/{channel}/{campaign}/[nonres+vbf_novhh_l1cvv1cv1]"))
    processes.append((r"HH ggF bbWW", "bbww", "/sig/{channel}/{campaign}/nonres/bbWW"))
    processes.append((r"HH ggF bb\tau\tau", "bbtt", "/sig/{channel}/{campaign}/nonres/bbtautau"))
    processes.append((r"HH ggF bbZZ", "bbzz", "/sig/{channel}/{campaign}/nonres/bbZZ"))
    processes.append(
        (r"Total background (using MC Fakes)", "bkg_mcfakes", "/bkg/{channel}/{campaign}/[prompt+nonprompt]")
    )
    processes.append(
        (
            r"Total background (using SSOS Fakes)",
            "bkg_ddfakes",
            "/bkg/{channel}/{campaign}/[prompt+ddFakes/[data+mc/prompt]]",
        )
    )
    processes.append((r"MC Fakes", "fakes_mc", "/bkg/{channel}/{campaign}/nonprompt/"))
    processes.append((r"SSOS Fakes", "fakes_dd", "/bkg/{channel}/{campaign}/ddFakes/[data+mc/prompt]"))

    distributions = list()
    # distributions.append(("Cut2TagMllSR1VBFVeto", "Lepton0d0sigBL_vs_Lepton1d0sigBL"))
    distributions.append(("Cut2TagMllSR1VBFVeto", "NN_SR1_Signal_Rebin_vs_Lepton1Pt"))

    output_dir = "results/2d_plots"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    for campaign in campaigns:
        for channel in channels:
            for cut, distribution in distributions:
                for process_title, process_name, process_path in processes:
                    make_2d_plot(
                        reader,
                        campaign,
                        channel,
                        cut,
                        distribution,
                        process_title,
                        process_name,
                        process_path,
                        output_dir,
                    )


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser(description="Make 2d plots")
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
