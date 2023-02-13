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


def create_cutflow_table(reader, campaign, channel, processes, cuts):
    header = ["Cut"] + list(zip(*processes)[0]) + ["s/b", "Z"]
    body = list()
    for cut_title, cut_name in cuts:
        if cut_title == "|":
            body.append("|")
            continue

        row = list()
        row.append(cut_title)

        s = 0
        b = 0
        sigma_b = 0

        for process_name, process_path in processes:
            process_path_formatted = process_path.format(campaign=campaign, channel=channel)
            counter = reader.getCounter(process_path_formatted, cut_name)

            if not counter:
                logging.error("Could not retrieve counter for '%s' - '%s'", process_path_formatted, cut_name)
                sys.exit(1)

            if process_name == "Data":
                row.append(r"{}".format(int(counter.getCounter())))
            else:
                row.append(r"{} $\pm$ {}".format(*pdgRound(counter.getCounter(), counter.getError())))

            if process_name == "Sig.":
                s = counter.getCounter()
            elif process_name == "Bkg.":
                b = counter.getCounter()
                sigma_b = counter.getError()

        if b > 0:
            s_over_b = "{:.7f}".format(s / b)
        else:
            s_over_b = "--"
        row.append(s_over_b)

        row.append("{:.5f}".format(asimov_sig(s, b, sigma_b)))

        body.append(row)

    return header, body


def write_cutflow_table(tex, campaign, channel, header, body):
    n_cols = len(header)

    tex.writeline(r"\begin{table}[htb]")
    tex.writeline(r"\caption{" + campaign + " -- $" + channel + "$ channel}")
    tex.writeline(r"\begin{center}")
    tex.writeline(r"\resizebox{\columnwidth}{!}{%}")
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
    tex.writeline(r"}")
    tex.writeline(r"\end{center}")
    tex.writeline(r"\end{table}")
    tex.writeline("\n\n\n\n")


def main(args):
    logging.info("Loading sample folder, this may take a while ...")
    samples = TQSampleFolder.loadSampleFolder(args.input_file + ":" + args.sample_folder)
    reader = TQSampleDataReader(samples)

    output_dir = "results/cutflow_1tag"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    channels = ["ee", "mm", "em", "me", "sf", "df", "ll"]
    campaigns = ["run2"]

    processes = list()
    processes.append(("$bbWW$", "/sig/{channel}/{campaign}/nonres/bbWW"))
    processes.append((r"$bb\tau\tau$", "/sig/{channel}/{campaign}/nonres/bbtautau"))
    processes.append(("$bbZZ$", "/sig/{channel}/{campaign}/nonres/bbZZ"))
    processes.append(("Top", "/bkg/{channel}/{campaign}/top/ttbar"))
    processes.append((r"Diboson", "/bkg/{channel}/{campaign}/diboson"))
    processes.append((r"$Z\rightarrow\ell\ell$", "/bkg/{channel}/{campaign}/[Zee+Zmm]"))
    processes.append((r"$Z\rightarrow\tau\tau$", "/bkg/{channel}/{campaign}/Ztt"))
    processes.append(("Fakes", "/bkg/{channel}/{campaign}/fakes"))
    processes.append((r"$H$", "/bkg/{channel}/{campaign}/Higgs"))

    processes.append(("Sig.", "/sig/{channel}/{campaign}/nonres"))
    processes.append(("Bkg.", "/bkg/{channel}/{campaign}"))
    processes.append(("Data", "/data/{channel}/{campaign}"))

    cuts = list()
    cuts.append(("2 b-tag SR1", "Cut2TagMllSR1"))
    cuts.append(("2 b-tag SR2", "Cut2TagMbbSR2"))
    cuts.append(("2 b-tag TopCR", "Cut2TagMllTopCR"))
    cuts.append(("2 b-tag ZllCR", "Cut2TagMbbZllCR"))
    cuts.append(("|", "|"))
    cuts.append(("1 b-tag SR1", "Cut1TagMllSR1"))
    cuts.append(("1 b-tag SR2", "Cut1TagMllSR2"))
    cuts.append(("1 b-tag TopCR", "Cut1TagMllTopCR"))

    output_file_path = os.path.join(output_dir, "cutflow.tex")
    with LaTeXFile.from_rel_path(output_file_path) as tex:
        tex.document_settings = ["12pt", "landscape"]
        tex.write_header()
        tex.writeline(r"\geometry{a3paper}")
        tex.begin_document()

        for campaign in campaigns:
            for channel in channels:
                logging.info("Creating cutflow for channel '%s'", channel)
                header, body = create_cutflow_table(
                    reader, campaign_to_sfpath(campaign), channel_to_sfpath(channel), processes, cuts
                )
                write_cutflow_table(tex, campaign_to_latex(campaign), channel_to_latex(channel), header, body)

        tex.end_document()
        tex.write_make_file()


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Make nice cutflow in latex")
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
