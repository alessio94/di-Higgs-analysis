#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import logging
import math
import os
import sys

from CAFbbll.latexfile import LaTeXFile
from CAFbbll.pdg_rounding import pdgRound


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


def create_cutflow_table(reader, campaign, channel, processes, cuts, raw_yields):
    header = ["Cut"] + list(zip(*processes)[0])
    body = list()
    for cut_title, cut_name in cuts:
        if cut_title == "|":
            body.append("|")
            continue

        row = list()
        row.append(cut_title)

        for process_name, process_path in processes:
            process_path_formatted = process_path.format(campaign=campaign, channel=channel)
            counter = reader.getCounter(process_path_formatted, cut_name)
            if not counter:
                logging.error("Could not get counter for process '%s' and cut '%s'", process_path_formatted, cut_name)
                sys.exit(1)

            if not raw_yields:
                row.append(r"{} $\pm$ {}".format(*pdgRound(counter.getCounter(), counter.getError())))
            else:
                row.append(r"{}".format(counter.getRawCounter()))

        body.append(row)

    return header, body


def write_cutflow_table(tex, campaign, channel, header, body):
    n_cols = len(header)

    tex.writeline(r"\begin{table}[H]")
    tex.writeline(r"\caption{" + campaign + " -- $" + channel + "$ channel}")
    tex.writeline(r"\begin{center}")
    tex.writeline(r"\begin{tabular}{@{}l" + "c" * n_cols + "@{}}")
    tex.writeline(r"\toprule")

    tex.writeline(" & ".join(header) + r" \\ \midrule")
    for row in body:
        if row == "|":
            tex.writeline(r"\midrule")
        elif isinstance(row, str):
            tex.writeline(r"\multicolumn{" + str(n_cols) + "}{c}{" + row + r"} \\")
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

    output_dir = "results/overlap/bbyy"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    channels = ["sf", "df", "ll"]
    campaigns = ["c16a", "c16d", "c16e", "run2"]

    processes = list()
    processes.append(("$bbWW$", "/sig/{channel}/{campaign}/nonres/bbWW"))
    processes.append((r"$bb\tau\tau$", "/sig/{channel}/{campaign}/nonres/bbtautau"))
    processes.append(("$bbZZ$", "/sig/{channel}/{campaign}/nonres/bbZZ"))
    processes.append((r"$bb\gamma\gamma$", "/sig/{channel}/{campaign}/nonres/bbyy"))

    cuts = list()
    cuts.append(("NLeptons=2", "CutBase"))
    cuts.append(("|", "|"))
    cuts.append(("2 b-tag SR1", "Cut2TagMllSR1"))
    cuts.append(("2 b-tag SR2", "Cut2TagMllSR2"))
    cuts.append(("2 b-tag TopCR", "Cut2TagMllTopCR"))
    cuts.append(("|", "|"))
    cuts.append(("1 b-tag SR1", "Cut1TagMllSR1"))
    cuts.append(("1 b-tag SR2", "Cut1TagMllSR2"))
    cuts.append(("1 b-tag TopCR", "Cut1TagMllTopCR"))

    if not args.raw:
        output_file_path = os.path.join(output_dir, "overlap_bbyy.tex")
    else:
        output_file_path = os.path.join(output_dir, "overlap_bbyy_raw.tex")
    with LaTeXFile.from_rel_path(output_file_path) as tex:
        tex.document_settings = ["12pt", "landscape"]
        tex.write_header()
        # tex.writeline(r"\geometry{a3paper}")
        tex.begin_document()

        for campaign in campaigns:
            for channel in channels:
                processes_final = list(processes)
                if campaign == "c16d":
                    processes_final.append((r"$bb\gamma\gamma$ (FS)", "/sig/{channel}/{campaign}/nonres/bbyy_fs"))
                logging.info("Creating cutflow for campaign '%s' and channel '%s'", campaign, channel)
                header, body = create_cutflow_table(
                    reader, campaign_to_sfpath(campaign), channel_to_sfpath(channel), processes_final, cuts, args.raw
                )
                write_cutflow_table(tex, campaign_to_latex(campaign), channel_to_latex(channel), header, body)

        tex.end_document()
        tex.write_make_file()


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Create yield table for bbyy overlap studies")
    PARSER.add_argument("input_file", metavar="INPUT_FILE", type=str, help="Input file")
    PARSER.add_argument(
        "--sample-folder", dest="sample_folder", type=str, help="Name of sample folder", default="samples"
    )
    PARSER.add_argument("--raw", dest="raw", action="store_true", help="Print raw yields", default=False)
    PARSER.add_argument("--debug", dest="debug", action="store_true", help="Print debug messages", default=False)
    ARGS = PARSER.parse_args()

    LOG_LEVEL = logging.DEBUG if ARGS.debug else logging.INFO
    logging.basicConfig(
        level=LOG_LEVEL, format="%(asctime)s - %(levelname)s - %(message)s", datefmt="%Y-%m-%d %H:%M:%S"
    )

    # only load QFramework after command line arguments have been parsed
    from QFramework import TQSampleFolder, TQSampleDataReader

    main(ARGS)
