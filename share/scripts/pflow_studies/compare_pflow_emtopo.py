#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import logging
import math
import os

from CAFbbll.latexfile import LaTeXFile
from CAFbbll.pdg_rounding import pdgRound


ALGOS = ["EMTopo", "EMPFlow"]


def asimov_sig(s, b, sigma_b=0):
    if sigma_b == 0:
        if s > 0 and b > 0:
            return math.sqrt(2 * ((s + b) * math.log(1 + s / b) - s))
        else:
            return 0
    else:
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


def create_cutflow_table(readers, campaign, channel, processes, cuts):
    header = ["Jet Algo"] + list(zip(*processes)[0])
    body = list()
    for cut_i, (cut_title, cut_name) in enumerate(cuts):
        if cut_i > 0:
            body.append("|")
        body.append(cut_title)

        for algo in ALGOS:
            row = list()
            row.append(algo)

            count_sig = 0
            count_ttbar = 0
            count_data = 0
            for process_name, process_path in processes:
                counter = readers[algo].getCounter(process_path.format(campaign=campaign, channel=channel), cut_name)

                if process_name == "Data":
                    row.append(r"{}".format(int(counter.getCounter())))
                else:
                    row.append(r"{} $\pm$ {}".format(*pdgRound(counter.getCounter(), counter.getError())))

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
    algo_filenames = dict()
    algo_filenames["EMTopo"] = "emtopo"
    algo_filenames["EMPFlow"] = "pflow"
    samples = dict()
    readers = dict()
    for algo in ALGOS:
        logging.info("Loading sample folder for %s, this may take a while ...", algo)
        file_path_template = "sampleFolders/analyzed/samples-analyzed-{algo_filename}.root"
        sample_folder = TQSampleFolder.loadSampleFolder(
            file_path_template.format(algo_filename=algo_filenames[algo]) + ":" + args.sample_folder
        )
        samples[algo] = sample_folder
        readers[algo] = TQSampleDataReader(sample_folder)

    output_dir = "results/pflow_emtopo_yields"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    channels = ["sf", "df"]
    campaigns = ["run2"]

    processes = list()
    processes.append(("Signal", "/sig/{channel}/{campaign}/nonres"))
    processes.append((r"$t\overline{t}$", "/bkg/{channel}/{campaign}/top/ttbar/nonallhad"))
    processes.append(("Data", "/data/{channel}/{campaign}"))

    cuts = list()
    cuts.append(("SR1", "CutMllSR1"))
    cuts.append(("SR2", "CutMllSR2"))

    output_file_path = os.path.join(output_dir, "comp_pflow_emtopo.tex")
    with LaTeXFile.from_rel_path(output_file_path) as tex:
        tex.document_settings = ["12pt", "a4paper"]
        tex.write_header()
        tex.begin_document()

        for campaign in campaigns:
            for channel in channels:
                logging.info("Creating cutflow for campaign '%s' and channel '%s'", campaign, channel)
                header, body = create_cutflow_table(
                    readers, campaign_to_sfpath(campaign), channel_to_sfpath(channel), processes, cuts
                )
                write_cutflow_table(tex, campaign_to_latex(campaign), channel_to_latex(channel), header, body)

        tex.end_document()
        tex.write_make_file()


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Create yield table for emtopo/pflow comparison studies.")
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
