#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import logging
import os
import sys

from CAFbbll.atlas import set_atlas_style
from ROOT import gROOT, gStyle
from ROOT import TFile, TCanvas, TLegend, TLatex
from ROOT import kRed, kBlue


PRW_RUNNUMBERS = dict()
PRW_RUNNUMBERS["mc16a"] = "284500"
PRW_RUNNUMBERS["mc16d"] = "300000"
PRW_RUNNUMBERS["mc16e"] = "310000"


def get_hist_from_file(filepath, dsid, campaign):
    hist_name = "PileupReweighting/pileup_chan{0}_run{1}".format(dsid, PRW_RUNNUMBERS[campaign])
    logging.info("Opening file '%s'", filepath)
    root_file = TFile.Open(filepath)
    try:
        logging.info("Getting histogram '%s'", hist_name)
        hist = root_file.Get(hist_name)
        if not hist:
            logging.error("Could not get histogram '%s' from file '%s'", hist_name, filepath)
            sys.exit(1)
        hist.SetDirectory(0)  # ensure that the histogram is kept in memory after the file is closed
    finally:
        root_file.Close()
    return hist


def main(args):
    gROOT.ProcessLine("gErrorIgnoreLevel = 1001;")  # disalbe INFO messages
    gROOT.SetBatch(True)  # enable batch mode
    gStyle.SetOptStat(0)  # disable statistics box
    set_atlas_style()

    hist_a = get_hist_from_file(args.input_file_a, args.dsid, args.campaign)
    hist_b = get_hist_from_file(args.input_file_b, args.dsid, args.campaign)

    logging.info("SumW from '%s': %f", args.input_file_a, hist_a.GetSumOfWeights())
    logging.info("SumW from '%s': %f", args.input_file_b, hist_b.GetSumOfWeights())

    canvas = TCanvas("canvas", "", 800, 600)
    hist_a.SetLineColor(kRed)
    hist_a.SetLineWidth(2)
    hist_a.GetXaxis().SetTitle("#mu")
    hist_a.GetYaxis().SetTitle("SumW(#mu)")
    hist_a.Draw("HIST")
    hist_b.SetLineColor(kBlue)
    hist_b.SetLineWidth(2)
    hist_b.SetLineStyle(2)
    hist_b.Draw("HIST SAME")

    hist_a.SetMaximum(max(hist_a.GetMaximum(), hist_b.GetMaximum()) * 1.1)

    legend = TLegend(0.65, 0.7, 0.92, 0.92)
    legend.SetBorderSize(0)
    legend.SetTextFont(42)
    legend.SetTextSize(0.04)
    legend.SetFillColor(-1)
    legend.AddEntry(hist_a, args.label_a, "l")
    legend.AddEntry(None, "#sum w = {0:.3g}".format(hist_a.GetSumOfWeights()), "")
    legend.AddEntry(hist_b, args.label_b, "l")
    legend.AddEntry(None, "#sum w = {0:.3g}".format(hist_b.GetSumOfWeights()), "")
    legend.Draw()

    latex = TLatex()
    latex.SetNDC()
    latex.SetTextFont(42)
    latex.DrawLatex(0.2, 0.85, args.campaign)

    canvas.Update()

    output_dir = "results/prw"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    output_path = os.path.join(output_dir, "compare_{0}_{1}.pdf".format(args.dsid, args.campaign))
    canvas.SaveAs(output_path)
    logging.info("Histogram saved at '%s'", output_path)


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Compare different PRW files")
    PARSER.add_argument("input_file_a", metavar="INPUT_FILE_A", type=str, help="First PRW file")
    PARSER.add_argument("input_file_b", metavar="INPUT_FILE_B", type=str, help="Second PRW file")
    PARSER.add_argument("--dsid", dest="dsid", type=str, help="DSID of sample", default="410472")
    PARSER.add_argument("--campaign", dest="campaign", type=str, help="Campaign", default="mc16a")
    PARSER.add_argument(
        "--label_a", dest="label_a", type=str, help="Label for first PRW file", default="Centrally stored"
    )
    PARSER.add_argument(
        "--label_b", dest="label_b", type=str, help="Label for second PRW file", default="Newly generated"
    )
    PARSER.add_argument("--debug", dest="debug", action="store_true", help="Print debug messages", default=False)
    ARGS = PARSER.parse_args()

    LOG_LEVEL = logging.DEBUG if ARGS.debug else logging.INFO
    logging.basicConfig(
        level=LOG_LEVEL, format="%(asctime)s - %(levelname)s - %(message)s", datefmt="%Y-%m-%d %H:%M:%S"
    )

    main(ARGS)
