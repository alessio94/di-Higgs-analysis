#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import collections
import fnmatch
import logging
import os

from ROOT import gROOT, gStyle
from ROOT import TCanvas, TPad, TLatex, TLegend
from ROOT import kBlack, kRed, kGreen, kBlue, kGray, kCyan, kOrange
from CAFbbll.atlas import set_atlas_style, draw_atlas_label, WIP
from CAFbbll.sample_folder_tools import get_sumw_from_samples, split_sample_path

PlotOptions = collections.namedtuple("PlotOptions", "line_color fill_style line_style title")

plot_options = dict()


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


def get_total_sumw_times_norm(samples, path):
    total_sumw_times_norm = 0
    for path_part in split_sample_path(path):
        campaign = path_part.split("/")[3]
        sumw_info = get_sumw_from_samples(samples, path_part)
        # norm is basically (xsec * kfactor * fitlereff) / sumw.
        # we want sumw * (xsec * kfactor * fitlereff) so we need to multiply with sumw^2
        total_sumw_times_norm += sum(
            [s[campaign].sumw * s[campaign].sumw * s[campaign].norm for s in sumw_info.values()]
        )
    return total_sumw_times_norm


def compute_efficiency(samples, reader, campaign, channel, processes, cuts):
    caf_campaign = str_to_caf_campaign(campaign)
    caf_channel = str_to_caf_channel(channel)

    results = list()

    print("")
    print("campaign: " + campaign + " - channel: " + channel)

    for process in processes:
        process_name, process_title, process_path = process
        process_path_formatted = process_path.format(campaign=caf_campaign, channel=caf_channel)

        if process_name == "total background":
            yield_sel_tot_bkg = reader.getCounter(process_path_formatted, cuts[1]).getCounter()

    for process in processes:
        process_name, process_title, process_path = process
        process_path_formatted = process_path.format(campaign=caf_campaign, channel=caf_channel)

        yield_base = reader.getCounter(process_path_formatted, cuts[0]).getCounter()
        yield_sel = reader.getCounter(process_path_formatted, cuts[1]).getCounter()

        # for getting the initial sum-of-weights only look at one channel
        # this operates on a file-level, and since each file exists in all channels
        # the sumw would be wrong by a factor of the number of requested channels
        yield_sumw = get_total_sumw_times_norm(samples, process_path.format(campaign=caf_campaign, channel="ee"))

        if yield_base > 0:
            effi = "{:.4f}".format(100 * yield_sel / yield_base)
        else:
            effi = "--"

        if yield_sumw > 0:
            effi_sumw = "{:.10f}".format(100 * yield_sel / yield_sumw)
        else:
            effi_sumw = "--"

        if yield_sel_tot_bkg > 0:
            fraction = "{:.4f}".format(100 * yield_sel / yield_sel_tot_bkg)
        else:
            fraction = "--"

        yield_sel_formatted = "{:.4f}".format(yield_sel)

        print("")
        print("process: " + process_name)
        print("yield for " + cuts[1] + " = " + yield_sel_formatted)
        print("efficiency for " + cuts[1] + "/" + cuts[0] + "  = " + effi + "%")
        print("efficiency for " + cuts[1] + "/ SumW = " + effi_sumw + "%")
        print("fraction wrt total background for " + cuts[1] + " = " + fraction + "%")
        results.append((process_name, effi, effi_sumw, yield_sel_formatted, fraction))
    return results


def write_efficiency(campaign, channel, cuts, results, output_dir):
    output_file_name = output_dir + "single-Higgs-study.txt"
    file = open(output_file_name, "w+")
    file.write("campaign: " + campaign + " - channel: " + channel + "\n")
    file.write("cut base: " + cuts[0] + " - cut sel: " + cuts[1] + "\n")
    file.write("process   -   effi*acc   -   effi_sumw   -  yield   -   fraction/tot bkg \n")
    for result in results:
        process_name, effi, effi_sumw, yield_sel, fraction = result
        file.write(
            process_name + "  -  " + effi + "% -  " + effi_sumw + "% -  " + yield_sel + "  -  " + fraction + "% \n"
        )
    file.close()


def main(args):
    gROOT.ProcessLine("gErrorIgnoreLevel = 1001;")  # disable INFO messages
    gROOT.SetBatch(True)  # enable batch mode
    gStyle.SetOptStat(0)  # disable statistics box
    set_atlas_style()

    cut_base = "Cut{0:s}".format(args.cut_base)
    cut_sel = "Cut{0:s}".format(args.cut_sel)

    logging.info("Loading sample folder, this may take a while ...")
    samples = TQSampleFolder.loadSampleFolder(args.input_file + ":" + args.sample_folder)
    reader = TQSampleDataReader(samples)

    output_dir = "results/single_Higgs_bkg/"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # channels = ["df", "sf", "ll"]
    channels = ["ll"]
    # campaigns = ["c16a","c16d","c16e","run2"]
    campaigns = ["run2"]

    cuts = list()
    cuts.append(cut_base)
    cuts.append(cut_sel)

    results = list()

    process_top = "/bkg/{channel}/{campaign}/top/[ttbar/nonallhad+stop+ttbarV]"
    process_fakes = (
        "/bkg/{channel}/{campaign}/fakes/[diboson+Wenu+Wmunu+Wtaunu+top/stop+top/ttbar/allhad+/top/ttbar/nonallhad]"
    )

    processes = list()
    processes.append(("signal", "HH signal", "/sig/{channel}/{campaign}/nonres"))
    processes.append(("bbtautau", "HH bb#tau#tau", "/sig/{channel}/{campaign}/nonres/bbtautau"))
    processes.append(("bbWW", "HH bbWW", "/sig/{channel}/{campaign}/nonres/bbWW"))
    processes.append(("bbZZ", "HH bbZZ", "/sig/{channel}/{campaign}/nonres/bbZZ"))

    processes.append(
        (
            "total background",
            "background",
            process_top + "+" + process_fakes + "+/bkg/{channel}/{campaign}/[Higgs+Zee+Zmm+Ztt+diboson]",
        )
    )
    processes.append(("single Higgs background", "single Higgs background", "/bkg/{channel}/{campaign}/Higgs"))

    processes.append(("Htautau", "H#tau#tau", "/bkg/{channel}/{campaign}/Higgs/tautau"))
    processes.append(("Htautau ggF", "H#tau#tau ggF", "/bkg/{channel}/{campaign}/Higgs/tautau/ggf"))
    processes.append(("Htautau VBF", "H#tau#tau VBF", "/bkg/{channel}/{campaign}/Higgs/tautau/vbf"))
    processes.append(("Htautau VH", "H#tau#tau VH", "/bkg/{channel}/{campaign}/Higgs/tautau/vh"))

    processes.append(("HWW", "HWW", "/bkg/{channel}/{campaign}/Higgs/WW"))
    processes.append(("HWW ggF", "HWW ggF", "/bkg/{channel}/{campaign}/Higgs/WW/ggf"))
    processes.append(("HWW VBF", "HWW VBF", "/bkg/{channel}/{campaign}/Higgs/WW/vbf"))

    processes.append(("ttH", "ttH", "/bkg/{channel}/{campaign}/Higgs/ttH/[346343+346344+346345]"))
    processes.append(("ttH allhad", "ttH allhad", "/bkg/{channel}/{campaign}/Higgs/ttH/346343"))
    processes.append(("ttH semilep", "ttH semilep", "/bkg/{channel}/{campaign}/Higgs/ttH/346344"))
    processes.append(("ttH dilep", "ttH dilep", "/bkg/{channel}/{campaign}/Higgs/ttH/346345"))

    for campaign in campaigns:
        for channel in channels:
            results = compute_efficiency(samples, reader, campaign, channel, processes, cuts)

    write_efficiency(campaign, channel, cuts, results, output_dir)


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("study single-Higgs background")
    PARSER.add_argument("input_file", metavar="INPUT_FILE", type=str, help="Input file")
    PARSER.add_argument("cut_base", metavar="CUT_BASE", type=str, help="Base, TriggerMatching")
    PARSER.add_argument("cut_sel", metavar="CUT_SEL", type=str, help="2TagMllSR1, 2TagMllSR2")
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
