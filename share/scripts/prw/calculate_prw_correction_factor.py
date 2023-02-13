#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
TODO
"""

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import logging
import os
import sys

from CAFbbll.atlas import set_atlas_style
from CAFbbll.sample_folder_tools import get_sumw_from_samples

from ROOT import gROOT, gStyle
from ROOT import TFile, TCanvas, TH1F, TLegend
from ROOT import kGreen, kCyan, kViolet


CAMPAIGNS = ["c16a", "c16d", "c16e"]

PRW_BASE_PATH = "/home/br1033/bbll/CxAODFramework/source/CxAODOperations_HH_bbll/data/PRW"

PRW_RUNNUMBERS = dict()
PRW_RUNNUMBERS["c16a"] = "284500"
PRW_RUNNUMBERS["c16d"] = "300000"
PRW_RUNNUMBERS["c16e"] = "310000"

LUMI = dict()
LUMI["c16a"] = 36207.66
LUMI["c16d"] = 44307.40
LUMI["c16e"] = 58450.10
LUMI["all"] = LUMI["c16a"] + LUMI["c16d"] + LUMI["c16e"]

HIST_COLORS = dict()
HIST_COLORS["c16a"] = kGreen
HIST_COLORS["c16d"] = kCyan
HIST_COLORS["c16e"] = kViolet


def isclose(a, b, rel_tol=1e-09, abs_tol=0.0):
    """Python 2 implementation of math.isclose"""
    return abs(a - b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol)


def get_sumw_from_prw(sample_infos):
    results = dict()
    for (dsid, sim_tag), sample_info_all_campaigns in sample_infos.items():
        results_key = (dsid, sim_tag)
        results[results_key] = dict()
        for campaign in CAMPAIGNS:
            if campaign not in sample_info_all_campaigns:
                results[results_key][campaign] = 0
                continue

            root_file_name = "pileup_m{0}_dsid{1}_{2}.root".format(campaign, dsid, sim_tag)
            root_file_path = os.path.join(PRW_BASE_PATH, root_file_name)
            if not os.path.exists(root_file_path):
                logging.info("Did not find file '%s'", root_file_path)
                results[results_key][campaign] = 0
                continue
            try:
                root_file = TFile.Open(root_file_path)
                if not root_file:
                    logging.error("Unexpected error while opening file '%s'", root_file_path)
                    sys.exit(1)

                hist_name = "PileupReweighting/pileup_chan{0}_run{1}".format(dsid, PRW_RUNNUMBERS[campaign])
                hist = root_file.Get(hist_name)
                if not hist:
                    logging.warning("Could not retrieve histogram '%s' from file '%s'", hist_name, root_file_path)
                    results[results_key][campaign] = 0
                    sys.exit(1)
                else:
                    results[results_key][campaign] = hist.GetSumOfWeights()
            finally:
                if root_file:
                    root_file.Close()

    for results_key in sorted(results.keys()):
        results[results_key]["all"] = sum([results[results_key][c] for c in CAMPAIGNS])

    return results


def calculate_correction_factors(samples, prw):
    results = dict()
    print(
        " dsid  | sim  | campaign |      PRW SumW     | PRW fraction |    Sample SumW    | Sample fraction | correction"
    )
    print(
        "------ | ---- | -------- | ----------------- | ------------ | ----------------- | --------------- | ----------"
    )
    for dsid, sim_tag in sorted(samples.keys()):
        key = (dsid, sim_tag)
        results[key] = dict()
        for campaign in CAMPAIGNS:
            try:
                prw_fraction = prw[key][campaign] / prw[key]["all"]
                sample_fraction = samples[key][campaign].sumw / samples[key]["all"]
                # lumi_fraction = LUMI[campaign] / LUMI["all"]
                results[key][campaign] = prw_fraction / sample_fraction
                print(
                    "{} | {:4} | {:8} | {:21.4f} | {:12.2f} | {:21.4f} | {:14.2f} | {:10.2f}".format(
                        dsid,
                        sim_tag,
                        "m" + campaign,
                        prw[key][campaign],
                        prw_fraction,
                        samples[key][campaign].sumw,
                        sample_fraction,
                        results[key][campaign],
                    )
                )
            except KeyError:
                pass
            except ZeroDivisionError:
                logging.error("Tried to divide by zero for dsid '%s' (%s) and campaign '%s'", dsid, sim_tag, campaign)
                pass

    return results


def print_correction_factors(factors):
    for (dsid, sim_tag), factors_per_dsid in sorted(factors.items()):
        dsid_info = "{0} ({1})".format(dsid, sim_tag)
        logging.debug(dsid_info)
        logging.debug("-" * len(dsid_info))
        for campaign, factor in factors_per_dsid.items():
            logging.debug("    %s: %.3g", campaign, factor)


def write_correction_factors_to_patch_file(factors, samples, replace_channel):
    file_path = "bbll/config/patches/analyze-patch-prw.txt"
    template = "<PRWFix = {0:.6f}> @ {1};\n"
    with open(file_path, "w") as patch_file:
        patch_file.write(template.format(1, "/."))
        for (dsid, sim_tag), factors_per_dsid in sorted(factors.items()):
            for campaign, factor in sorted(factors_per_dsid.items()):
                if not isclose(1, factor):
                    try:
                        path = samples[(dsid, sim_tag)][campaign].path.replace("/{0}/".format(replace_channel), "/?/")
                        patch_file.write(template.format(factor, path))
                    except KeyError:
                        pass


def write_correction_factors_to_text_file(factors, samples, prw):
    file_path = "results/prw/prw_fix.txt"
    with open(file_path, "w") as text_file:
        for (dsid, sim_tag), factors_per_dsid in sorted(factors.items()):
            dsid_info = "{0} ({1})".format(dsid, sim_tag)
            text_file.write("{0}\n".format(dsid_info))
            text_file.write("{0}\n".format("-" * len(dsid_info)))
            for campaign, factor in sorted(factors_per_dsid.items()):
                try:
                    text_file.write(
                        "  - {0}: AOD: {1:f}, PRW: {2:f}, factor: {3:.6f}\n".format(
                            campaign, samples[(dsid, sim_tag)][campaign].sumw, prw[(dsid, sim_tag)][campaign], factor
                        )
                    )
                except KeyError:
                    pass


def write_correction_factors_to_histgram(factors):
    hists = dict()
    for campaign in CAMPAIGNS:
        hists[campaign] = TH1F("prw_correction_{0}".format(campaign), "", 25, 0, 2.5)
        hists[campaign].SetLineColor(HIST_COLORS[campaign])
    for (dsid, _), factors_per_dsid in sorted(factors.items()):
        for campaign, factor in factors_per_dsid.items():
            hists[campaign].Fill(factor)

    canvas = TCanvas("canvas", "", 800, 600)
    first = True
    hist_max = max([hists[c].GetMaximum() for c in CAMPAIGNS]) * 1.1
    for campaign in CAMPAIGNS:
        if first:
            draw_args = "HIST"
            first = False
            hists[campaign].SetMaximum(hist_max)
            hists[campaign].GetXaxis().SetTitle("correction factor")
            hists[campaign].GetYaxis().SetTitle("number of samples")
        else:
            draw_args = "HIST SAME"
        hists[campaign].Draw(draw_args)

    legend = TLegend(0.75, 0.75, 0.92, 0.92)
    legend.SetBorderSize(0)
    legend.SetTextFont(42)
    legend.SetTextSize(0.04)
    legend.SetFillColor(-1)
    for campaign in CAMPAIGNS:
        legend.AddEntry(hists[campaign], "m" + campaign, "l")
    legend.Draw()

    output_path = "results/prw/prw_correction_factors.pdf"
    canvas.SaveAs(output_path)
    logging.info("Histogram saved at '%s'", output_path)


def main(args):
    gROOT.ProcessLine("gErrorIgnoreLevel = 1001;")  # disalbe INFO messages
    gROOT.SetBatch(True)  # enable batch mode
    gStyle.SetOptStat(0)  # disable statistics box
    set_atlas_style()

    base_samples = TQSampleFolder.loadSampleFolder(args.input_file + ":" + args.sample_folder)
    is_fakes_analysis = "fakes" in os.path.basename(args.input_file)
    if is_fakes_analysis:
        sample_paths = ["/sig/eee", "/bkg/eee", "/oth/eee", "/sys/eee"]
    else:
        sample_paths = ["/sig/ee", "/bkg/ee", "/oth/ee", "/sys/ee"]

    sumw_samples = dict()
    for sample_path in sample_paths:
        for dsid, sample_info in get_sumw_from_samples(base_samples, sample_path, CAMPAIGNS).items():
            fastsim_info = None
            for campaign in sample_info:
                if campaign == "all":
                    continue
                this_fastsim_info = "AFII" if sample_info[campaign].is_fastsim else "FS"
                if fastsim_info is None:
                    fastsim_info = this_fastsim_info
                elif fastsim_info != this_fastsim_info:
                    logging.error("Found mixed fullsim/fastsim sample, this doesn't look right!\n%s", sample_info)
                    sys.exit(1)
            sumw_samples[(dsid, fastsim_info)] = sample_info

    sumw_prw = get_sumw_from_prw(sumw_samples)

    correction_factors = calculate_correction_factors(sumw_samples, sumw_prw)

    print_correction_factors(correction_factors)
    write_correction_factors_to_patch_file(correction_factors, sumw_samples, "eee" if is_fakes_analysis else "ee")
    if not os.path.exists("results/prw"):
        os.makedirs("results/prw")
    write_correction_factors_to_text_file(correction_factors, sumw_samples, sumw_prw)
    write_correction_factors_to_histgram(correction_factors)


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Calculate the pile-up reweighting correction factor")
    PARSER.add_argument("input_file", metavar="INPUT_FILE", type=str, help="Input TQSampleFolder")
    PARSER.add_argument("--sample-folder", type=str, help="Name of sample folder", default="samples")
    PARSER.add_argument("--hist-name", type=str, help="Name of metadata histogram", default="MetaData_EventCount")
    PARSER.add_argument("--bin", type=int, help="Bin of initial sum-of-weights value in metadata histogram", default=4)
    PARSER.add_argument("--debug", dest="debug", action="store_true", help="Print debug messages", default=False)
    ARGS = PARSER.parse_args()

    LOG_LEVEL = logging.DEBUG if ARGS.debug else logging.INFO
    logging.basicConfig(
        level=LOG_LEVEL, format="%(asctime)s - %(levelname)s - %(message)s", datefmt="%Y-%m-%d %H:%M:%S"
    )

    # only load QFramework after command line arguments have been parsed
    from QFramework import TQSampleFolder

    main(ARGS)
