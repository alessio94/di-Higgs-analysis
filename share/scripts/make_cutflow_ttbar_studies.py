#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import logging
import math
import os
import sys

from CAFbbll.latexfile import LaTeXFile
from CAFbbll.pdg_rounding import pdgRound


def asimov_sig(s, b, sigma_b):
    sigma_b_squared = sigma_b ** 2

    if s > 0 and b > 0 and sigma_b_squared > 0:
        first = (s + b) * math.log((s + b) * (b + sigma_b_squared) / (b ** 2 + (s + b) * sigma_b_squared))
        second = b ** 2 / sigma_b_squared * math.log(1 + sigma_b_squared * s / (b * (b + sigma_b_squared)))
        if first > second:
            sig = math.sqrt(2 * (first - second))
        else:
            sig = 0
        return sig
    else:
        return 0


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
        return r"Run 2"
    elif campaign == "c16a":
        return "mc16a"
    elif campaign == "c16d":
        return "mc16d"
    elif campaign == "c16e":
        return "mc16e"
    else:
        return campaign


def campaign_to_sfpath(campaign):
    if campaign == "run2":
        return "[c16a+c16d+c16e]"
    else:
        return campaign


def create_cutflow_table(reader, channel, cut, campaigns, processes):
    header = ["Process"] + [campaign_to_latex(c) for c in campaigns] + ["mc16d/mc16a", "mc16e/mc16a", "Run 2/mc16a"]
    body = list()
    cut_title, cut_name = cut
    for process_name, process_path in processes:
        if cut_title == "|":
            body.append("|")
            continue

        row = list()
        row.append(process_name)

        yield_mc16a = 0
        yield_mc16d = 0
        yield_mc16e = 0
        yield_run2 = 0

        for campaign in campaigns:
            process_path_formatted = process_path.format(campaign=campaign_to_sfpath(campaign), channel=channel)
            counter = reader.getCounter(process_path_formatted, cut_name)

            if not counter:
                logging.error("Could not retrieve counter for '%s' - '%s'", process_path_formatted, cut_name)
                sys.exit(1)

            if process_name == "Signal":
                row.append(pdgRound(counter.getCounter(), counter.getError())[0])
            else:
                row.append(r"{}".format(int(counter.getCounter())))

            if campaign == "c16a":
                yield_mc16a = counter.getCounter()
            elif campaign == "c16d":
                yield_mc16d = counter.getCounter()
            elif campaign == "c16e":
                yield_mc16e = counter.getCounter()
            elif campaign == "run2":
                yield_run2 = counter.getCounter()

        row.append("{:.2f}".format(yield_mc16d / yield_mc16a))
        row.append("{:.2f}".format(yield_mc16e / yield_mc16a))
        row.append("{:.2f}".format(yield_run2 / yield_mc16a))

        body.append(row)

    return header, body


def write_cutflow_table(tex, cut, channel, header, body):
    n_cols = len(header)

    tex.writeline(r"\begin{table}[H]")
    tex.writeline(r"\caption{" + cut + " -- $" + channel + "$ channel}")
    tex.writeline(r"\begin{center}")
    tex.writeline(r"\begin{tabular}{@{}l" + "c" * n_cols + "@{}}")
    tex.writeline(r"\toprule")

    tex.writeline(" & ".join(header) + r" \\ \midrule")
    for row in body:
        if row == "|":
            tex.writeline(r"\midrule")
        else:
            tex.writeline(" & ".join(row) + r" \\")

    tex.writeline(r"\bottomrule")
    tex.writeline(r"\end{tabular}")
    tex.writeline(r"\end{center}")
    tex.writeline(r"\end{table}")
    tex.writeline("\n\n\n\n")


def main(args):
    logging.info("Loading sample folder, this may take a while ...")
    samples = TQSampleFolder.loadSampleFolder(args.input_file + ":" + args.sample_folder)
    reader = TQSampleDataReader(samples)

    output_dir = "results/cutflow_ttbar_studies-channels"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    channels = ["ll", "sf", "df", "ee", "mm", "em", "me"]
    # channels = ["ll"]
    campaigns = ["c16a", "c16d", "c16e", "run2"]

    processes = list()
    processes.append((r"$t\overline{t}$ nonallhad (DecayCode)", "/bkg/{channel}/{campaign}/top/ttbar/nonallhad"))
    processes.append((r"$t\overline{t}$ nonallhad (MCTC)", "/bkg/{channel}/{campaign}/top/ttbar/nonallhad_MCTC"))
    processes.append((r"$t\overline{t}$ dilepton", "/bkg/{channel}/{campaign}/top/ttbar/dilepton"))
    processes.append(("Signal", "/sig/{channel}/{campaign}/nonres"))
    processes.append((r"$Z\rightarrow\ell\ell$", "/bkg/{channel}/{campaign}/[Zee+Zmm]"))
    processes.append(("Data", "/data/{channel}/{campaign}"))

    cuts = list()
    # cuts.append(("Exactly 2 leptons", "CutSplitNonAllHadTTBar_MCTC"))
    # cuts.append(("Trigger selection", "CutTriggerMatching"))
    # cuts.append(("Exactly 2 b-jets", "CutNBJets2"))
    # cuts.append(("Exactly 1 b-jet", "CutNBJets1"))
    cuts.append(("2 b-tag SR1", "Cut2TagMllSR1"))
    # cuts.append(("2 b-tag SR2", "Cut2TagMllSR2"))
    # cuts.append(("2 b-tag TopCR", "Cut2TagMllTopCR"))
    # cuts.append(("1 b-tag SR1", "Cut1TagMllSR1"))
    # cuts.append(("1 b-tag SR2", "Cut1TagMllSR2"))
    # cuts.append(("1 b-tag TopCR", "Cut1TagMllTopCR"))

    output_file_path = os.path.join(output_dir, "cutflow.tex")
    with LaTeXFile.from_rel_path(output_file_path) as tex:
        tex.document_settings = ["12pt", "landscape"]
        tex.write_header()
        tex.begin_document()

        for channel in channels:
            for cut in cuts:
                logging.info("Creating cutflow for channel '%s' and cut '%s'", channel, cut[1])
                header, body = create_cutflow_table(reader, channel_to_sfpath(channel), cut, campaigns, processes)
                write_cutflow_table(tex, cut[0], channel_to_latex(channel), header, body)

        tex.end_document()
        tex.write_make_file()


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Make nice cutflow in latex for ttbar studies")
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
