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
    header = ["Process"] + list(zip(*cuts)[0])
    body = list()
    for process_name, process_path in processes:
        row = list()
        row.append(process_name)

        for cut_title, cut_name in cuts:
            counter = reader.getCounter(process_path.format(campaign=campaign, channel=channel), cut_name)

            if process_name == "Data":
                row.append(r"{}".format(int(counter.getCounter())))
            else:
                if raw_counts:
                    row.append("{}".format(counter.getRawCounter()))
                else:
                    row.append(r"{} $\pm$ {}".format(*pdgRound(counter.getCounter(), counter.getError())))

        body.append(row)

    return header, body


def write_cutflow_table(tex, campaign, channel, header, body):
    n_cols = len(header)

    tex.writeline(r"\begin{table}[htb]")
    tex.writeline(r"\caption{" + campaign + " -- $" + channel + "$ channel}")
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

    channels = ["ll"]
    campaigns = ["run2"]

    processes = list()
    processes.append((r"$Z\to{}\ell\ell$ (HF) -- main", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/hf/main"))
    processes.append((r"$Z\to{}\ell\ell$ (HF) -- low mass", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/hf/lowmass"))
    processes.append(
        (r"$Z\to{}\ell\ell$ (HF) -- very low mass", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/hf/verylowmass")
    )
    processes.append((r"$Z\to{}\ell\ell$ (HF) -- EW", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/hf/ew"))
    processes.append((r"$Z\to{}\ell\ell$ (HF) -- total", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/hf"))
    processes.append((r"$Z\to{}\ell\ell$ (LF) -- main", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/lf/main"))
    processes.append((r"$Z\to{}\ell\ell$ (LF) -- low mass", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/lf/lowmass"))
    processes.append(
        (r"$Z\to{}\ell\ell$ (LF) -- very low mass", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/lf/verylowmass")
    )
    processes.append((r"$Z\to{}\ell\ell$ (LF) -- EW", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/lf/ew"))
    processes.append((r"$Z\to{}\ell\ell$ (LF) -- total", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/lf"))
    processes.append((r"$Z\to{}\tau\tau$ (HF)", "/bkg/{channel}/{campaign}/prompt/Ztt/hf"))
    processes.append((r"$Z\to{}\tau\tau$ (LF)", "/bkg/{channel}/{campaign}/prompt/Ztt/lf"))

    cuts = list()
    cuts.append(("SR1+VBFVeto", "Cut2TagMllSR1VBFVeto"))
    cuts.append(("SR1+VBFVeto -- SF", "Cut2TagMllSR1VBFVetoSF"))
    cuts.append(("SR1+VBFVeto -- DF", "Cut2TagMllSR1VBFVetoDF"))
    cuts.append(("SR2", "Cut2TagMbbSR2"))
    cuts.append(("TTbarCR", "Cut2TagMBLTTbarCR"))
    cuts.append(("WtCR", "Cut2TagMBLWtCR"))
    cuts.append(("ZllCR", "Cut2TagMbbZllCR"))

    for version in ["raw", "weighted"]:
        raw_counts = version == "raw"
        output_dir = "results/cutflow_zjets_samplesplit_{:s}".format(version)
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
                    logging.info(
                        "Creating cutflow for campaign '%s' channel '%s' (raw counts=%s)", campaign, channel, raw_counts
                    )
                    header, body = create_cutflow_table(
                        reader, campaign_to_sfpath(campaign), channel_to_sfpath(channel), processes, cuts, raw_counts
                    )
                    write_cutflow_table(tex, campaign_to_latex(campaign), channel_to_latex(channel), header, body)

            tex.end_document()
            tex.write_make_file()


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser(description="Create cutflow table with Z+jets breakdown")
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
