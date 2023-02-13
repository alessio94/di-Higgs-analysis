#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import logging
import math
import os

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


def create_cutflow_table(reader, campaign, channel, processes, cuts, raw_counts=True):
    header = ["Sample"]
    body = list()
    first_row = True
    for process_name, process_path in processes:
        row = list()
        row.append(process_name)

        for cut_title, cut_name in cuts:
            if first_row:
                header.append(cut_title)
            process_path_formatted = process_path.format(campaign=campaign, channel=channel)
            counter = reader.getCounter(process_path_formatted, cut_name)

            if not counter:
                logging.error("Could not retrieve counter for path '%s' and cut '%s'", process_path_formatted, cut_name)

            if process_name == "Data":
                row.append(r"{}".format(int(counter.getCounter())))
            else:
                if raw_counts:
                    row.append("{}".format(counter.getRawCounter()))
                else:
                    if counter.getCounter() == 0:
                        row.append("--")
                    else:
                        row.append(r"{} $\pm$ {}".format(*pdgRound(counter.getCounter(), counter.getError())))

        body.append(row)
        first_row = False

    return header, body


def write_cutflow_table(tex, version, campaign, channel, header, body):
    n_cols = len(header)

    tex.writeline(r"\begin{table}[htb]")
    tex.writeline(r"\caption{" + version + " -- " + campaign + " -- $" + channel + "$ channel}")
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

    channels = ["ll"]
    campaigns = ["run2"]

    processes = list()
    processes.append((r"WH (incl.)", "/oth/{channel}/{campaign}/bkg/prompt/Higgs/vh/wh"))
    processes.append((r"WH (indiv.)", "/bkg/{channel}/{campaign}/prompt/Higgs/?/vh/wh"))
    processes.append((r"ZH (incl.)", "/oth/{channel}/{campaign}/bkg/prompt/Higgs/vh/zh"))
    processes.append((r"ZH (indiv.)", "/bkg/{channel}/{campaign}/prompt/Higgs/?/vh/zh"))

    cut_versions = list()
    cut_versions.append(
        (
            "sr1",
            [
                (r"$H \to cc$", "SR1Hcc"),
                (r"$H \to bb$", "SR1Hbb"),
                (r"$H \to \tau\tau$", "SR1Htautau"),
                (r"$H \to gg$", "SR1Hgg"),
                (r"$H \to ZZ$", "SR1HZZ"),
                (r"$H \to WW$", "SR1HWW"),
                ("total", "Cut2TagMllSR1VBFVeto"),
            ],
        )
    )
    for bin in range(1, 17):
        cut_versions.append(
            (
                "sr1bin{}".format(bin),
                [
                    (r"$H \to cc$", "SR1NNBin{}Hcc".format(bin)),
                    (r"$H \to bb$", "SR1NNBin{}Hbb".format(bin)),
                    (r"$H \to \tau\tau$", "SR1NNBin{}Htautau".format(bin)),
                    (r"$H \to gg$", "SR1NNBin{}Hgg".format(bin)),
                    (r"$H \to ZZ$", "SR1NNBin{}HZZ".format(bin)),
                    (r"$H \to WW$", "SR1NNBin{}HWW".format(bin)),
                    ("total", "Cut2TagMllSR1VBFVetoNNBin{}".format(bin)),
                ],
            )
        )

    for version in ["raw", "weighted"]:
        raw_counts = version == "raw"
        output_dir = "results/cutflow_vh_truth_study_{:s}".format(version)
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)

        output_file_path = os.path.join(output_dir, "cutflow.tex")
        with LaTeXFile.from_rel_path(output_file_path) as tex:
            tex.document_settings = ["12pt", "landscape"]
            tex.write_header()
            tex.writeline(r"\geometry{a3paper}")
            tex.begin_document()

            for campaign in campaigns:
                for channel in channels:
                    for cut_version, cuts in cut_versions:
                        logging.info(
                            "Creating cutflow '%s' for campaign '%s' channel '%s' (raw counts=%s)",
                            cut_version,
                            campaign,
                            channel,
                            raw_counts,
                        )
                        header, body = create_cutflow_table(
                            reader,
                            campaign_to_sfpath(campaign),
                            channel_to_sfpath(channel),
                            processes,
                            cuts,
                            raw_counts,
                        )
                        write_cutflow_table(
                            tex, cut_version, campaign_to_latex(campaign), channel_to_latex(channel), header, body
                        )

            tex.end_document()
            tex.write_make_file()


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser(description="Create cutflow table with fake breakdown")
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
