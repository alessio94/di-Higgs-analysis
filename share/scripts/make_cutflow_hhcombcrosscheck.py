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
    header = ["Process"] + list(zip(*cuts)[0]) + [r"$\varepsilon_{\text{SR1}}$", r"$\varepsilon_{\text{SR2}}$"]
    body = list()
    for process_name, process_path in processes:
        if process_name == "|":
            body.append("|")
            continue

        row = list()
        row.append(process_name)

        yields_cxaod = 0
        yields_sr1 = 0
        yields_sr2 = 0

        for cut_title, cut_name in cuts:
            if "campaign" in process_path:
                process_path_replaced = process_path.format(channel=channel, campaign=campaign)
            else:
                process_path_replaced = process_path.format(channel=channel)

            if process_name == "302261":
                counter = TQCounter(TString("fake"), 0, 0, 0)
            else:
                counter = reader.getCounter(process_path_replaced, cut_name)

            if raw_counts:
                row.append("{}".format(counter.getRawCounter()))
                if cut_name == "CutApplyTTBarSplitting":
                    yields_cxaod = counter.getRawCounter()
                elif cut_name == "Cut2TagMllSR1":
                    yields_sr1 = counter.getRawCounter()
                elif cut_name == "Cut2TagMbbSR1":
                    yields_sr2 = counter.getRawCounter()
            else:
                row.append(r"{} $\pm$ {}".format(*pdgRound(counter.getCounter(), counter.getError())))
                if cut_name == "CutApplyTTBarSplitting":
                    yields_cxaod = counter.getCounter()
                elif cut_name == "Cut2TagMllSR1":
                    yields_sr1 = counter.getCounter()
                elif cut_name == "Cut2TagMbbSR2":
                    yields_sr2 = counter.getCounter()

        if yields_cxaod != 0:
            eff_sr1 = "{:.3f}".format(yields_sr1 / yields_cxaod)
            eff_sr2 = "{:.3f}".format(yields_sr2 / yields_cxaod)
        else:
            eff_sr1 = "--"
            eff_sr2 = "--"
        row.append(eff_sr1)
        row.append(eff_sr2)

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
    processes.append((r"600047 ($bbWW, \kappa_\lambda=1$)", "/sys/{channel}/c16a/sig/nonres/herwig7/bbWW"))
    processes.append((r"600045 ($bb\tau\tau, \kappa_\lambda=1$)", "/sys/{channel}/c16a/sig/nonres/herwig7/bbtautau"))
    processes.append((r"600049 ($bbZZ, \kappa_\lambda=1$)", "/sys/{channel}/c16a/sig/nonres/herwig7/bbZZ"))
    processes.append((r"450628 ($bbWW, m_X=500$)", "/sig/{channel}/c16a/res/500/bbWW"))
    processes.append((r"451712 ($bb\tau\tau, m_X=500$)", "/sig/{channel}/c16a/res/500/bbtautau"))
    processes.append((r"450656 ($bbZZ, m_X=500$)", "/sig/{channel}/c16a/res/500/bbZZ"))
    processes.append((r"450259 ($bbbb, m_X=1000$)", "/oth/{channel}/c16a/HHcrosscheck/450259"))
    processes.append((r"450535 ($bb\gamma\gamma, m_X=500$)", "/oth/{channel}/c16a/HHcrosscheck/450535"))
    processes.append((r"600051 ($bb\gamma\gamma, \kappa_\lambda=1$)", "/oth/{channel}/c16a/HHcrosscheck/600051"))
    processes.append((r"301216", "/oth/{channel}/{campaign}/HHcrosscheck/301216"))
    processes.append((r"301221", "/oth/{channel}/{campaign}/HHcrosscheck/301221"))
    processes.append((r"301243", "/oth/{channel}/{campaign}/HHcrosscheck/301243"))
    processes.append((r"301247", "/oth/{channel}/{campaign}/HHcrosscheck/301247"))
    processes.append((r"301323", "/oth/{channel}/{campaign}/HHcrosscheck/301323"))
    processes.append((r"301325", "/oth/{channel}/{campaign}/HHcrosscheck/301325"))
    processes.append((r"301329", "/oth/{channel}/{campaign}/HHcrosscheck/301329"))
    processes.append((r"301826", "/oth/{channel}/{campaign}/HHcrosscheck/301826"))
    processes.append((r"302111", "/oth/{channel}/{campaign}/HHcrosscheck/302111"))
    processes.append((r"302136", "/oth/{channel}/{campaign}/HHcrosscheck/302136"))
    processes.append((r"302186", "/oth/{channel}/{campaign}/HHcrosscheck/302186"))
    processes.append((r"302211", "/oth/{channel}/{campaign}/HHcrosscheck/302211"))
    processes.append((r"302236", "/oth/{channel}/{campaign}/HHcrosscheck/302236"))
    processes.append((r"302261", "/oth/{channel}/{campaign}/HHcrosscheck/302261"))
    processes.append((r"302286", "/oth/{channel}/{campaign}/HHcrosscheck/302286"))
    processes.append((r"302336", "/oth/{channel}/{campaign}/HHcrosscheck/302336"))
    processes.append((r"302361", "/oth/{channel}/{campaign}/HHcrosscheck/302361"))
    processes.append((r"302386", "/oth/{channel}/{campaign}/HHcrosscheck/302386"))
    processes.append((r"302411", "/oth/{channel}/{campaign}/HHcrosscheck/302411"))
    processes.append((r"302436", "/oth/{channel}/{campaign}/HHcrosscheck/302436"))
    processes.append((r"302482", "/oth/{channel}/{campaign}/HHcrosscheck/302482"))
    processes.append((r"302719", "/oth/{channel}/{campaign}/HHcrosscheck/302719"))
    processes.append((r"306136", "/oth/{channel}/{campaign}/HHcrosscheck/306136"))
    processes.append((r"307018", "/oth/{channel}/{campaign}/HHcrosscheck/307018"))
    processes.append((r"308488", "/oth/{channel}/{campaign}/HHcrosscheck/308488"))

    cuts = list()
    cuts.append(("CxAOD selection", "CutApplyTTBarSplitting"))
    cuts.append(("SR1", "Cut2TagMllSR1"))
    cuts.append(("SR2", "Cut2TagMbbSR2"))
    cuts.append(("TopCR", "Cut2TagMllTopCR"))
    cuts.append(("ZllCR", "Cut2TagMbbZllCR "))

    for version in ["raw", "weighted"]:
        raw_counts = version == "raw"
        output_dir = "results/cutflow_hhcombcrosscheck_{:s}".format(version)
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)

        output_file_path = os.path.join(output_dir, "cutflow.tex")
        with LaTeXFile.from_rel_path(output_file_path) as tex:
            tex.document_settings = ["12pt", "landscape"]
            tex.write_header()
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
    PARSER = argparse.ArgumentParser(description="Create cutflow table with samples for HHComb overlay check")
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
    from QFramework import TQSampleFolder, TQSampleDataReader, TQCounter
    from ROOT import TString

    main(ARGS)
