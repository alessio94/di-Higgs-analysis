#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import logging
import math
import os

from CAFbbll.latexfile import LaTeXFile
from CAFbbll.pdg_rounding import pdgRound


ISO = ["PflowLoose_VarRad", "PflowLoose_FixedRad", "PflowTight_VarRad", "PflowTight_FixedRad"]


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

    header = (
        ["Muon Isolation WP"]
        + list(zip(*processes)[0])
        + [
            r"$\epsilon_{s}$",
            r"$\epsilon_\text{Top}$",
            r"$\epsilon_\text{Bkg}$",
            r"$\epsilon_\text{Data}$",
            "fakes/b",
            "s/b",
            "Z",
        ]
    )
    body = list()
    for cut_i, (cut_title, cut_names) in enumerate(cuts):
        ref_sig_count = 0
        ref_ttbar_count = 0
        ref_bkg_count = 0
        ref_data_count = 0
        if cut_i > 0:
            body.append("|")
        body.append(cut_title)

        for mu_iso in ISO:
            row = list()
            if mu_iso.find("_") > 0:
                str_list = list(mu_iso)
                str_list.insert(mu_iso.find("_"), "\\")
                row.append("".join(str_list))
            else:
                row.append(mu_iso)

            s = 0
            fakes = 0
            ttbar_count = 0
            b = 0
            data_count = 0
            for process_name, process_path in processes:
                for n_name, cut_name in enumerate(cut_names):
                    if n_name == 0:
                        counter = readers[mu_iso].getCounter(
                            process_path.format(campaign=campaign, channel=channel), cut_name
                        )
                    else:
                        counter.add(
                            readers[mu_iso].getCounter(
                                process_path.format(campaign=campaign, channel=channel), cut_name
                            )
                        )

                if process_name == "Data":
                    # if "BDT" in cut_name:
                    #    row.append("--")
                    # else:
                    row.append(r"{}".format(int(counter.getCounter())))
                else:
                    row.append(r"{} $\pm$ {}".format(*pdgRound(counter.getCounter(), counter.getError())))

                if process_name == "Signal":
                    s = counter.getCounter()
                    if mu_iso == ISO[0]:
                        ref_sig_count = s
                elif process_name == "Top":
                    ttbar_count = counter.getCounter()
                    if mu_iso == ISO[0]:
                        ref_ttbar_count = ttbar_count
                elif process_name == "Fakes":
                    fakes = counter.getCounter()
                elif process_name == "Bkg.":
                    b = counter.getCounter()
                    if mu_iso == ISO[0]:
                        ref_bkg_count = b
                elif process_name == "Data":
                    data_count = counter.getCounter()
                    if mu_iso == ISO[0]:
                        ref_data_count = data_count

            if ref_sig_count > 0:
                eff_sig = "{:.3f}".format(s / ref_sig_count)
            else:
                eff_sig = "--"
            if ref_ttbar_count > 0:
                eff_ttbar = "{:.3f}".format(ttbar_count / ref_ttbar_count)
            else:
                eff_ttbar = "--"
            if ref_bkg_count > 0:
                eff_bkg = "{:.3f}".format(b / ref_bkg_count)
            else:
                eff_bkg = "--"
            if ref_data_count > 0:
                eff_data = "{:.3f}".format(data_count / ref_data_count)
            else:
                eff_data = "--"
            if b > 0:
                fake_fraction = "{:.4f}".format(fakes / b)
                s_over_b = "{:.8f}".format(s / b)
            else:
                fake_fraction = "--"
                s_over_b = "--"
            row.append(eff_sig)
            row.append(eff_ttbar)
            row.append(eff_bkg)
            row.append(eff_data)
            row.append(fake_fraction)
            row.append(s_over_b)

            row.append("{:.6f}".format(asimov_sig(s, b)))

            body.append(row)

    return header, body


def write_cutflow_table(tex, campaign, channel, header, body):
    n_cols = len(header)

    tex.writeline(r"\begin{table}[H]")
    tex.writeline(r"\caption{" + campaign + " -- $" + channel + "$ channel}")
    tex.writeline(r"\begin{center}")
    tex.writeline(r"\resizebox{\columnwidth}{!}{%}")
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
    tex.writeline(r"}")
    tex.writeline(r"\end{center}")
    tex.writeline(r"\end{table}")
    tex.writeline("\n\n\n\n")


def main(args):
    samples = dict()
    readers = dict()
    for mu_iso in ISO:
        logging.info("Loading sample folder for iso=%s, this may take a while ...", mu_iso)
        file_path_template = "sampleFolders/analyzed/samples-analyzed-object_defs-mu_{iso}.root"
        sample_folder = TQSampleFolder.loadSampleFolder(
            file_path_template.format(iso=mu_iso) + ":" + args.sample_folder
        )
        samples[mu_iso] = sample_folder
        readers[mu_iso] = TQSampleDataReader(sample_folder)

    output_dir = "results/muon_iso_study/muons"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    channels = ["sf", "df", "ll"]
    campaigns = ["run2"]

    process_fakes = ()
    processes = list()
    processes.append(("Signal", "/sig/{channel}/{campaign}/nonres"))
    processes.append(("Top", "/bkg/{channel}/{campaign}/prompt/top/ttbar"))
    processes.append(("Fakes", "/bkg/{channel}/{campaign}/ddFakes/[data+mc/prompt]"))
    processes.append(("Bkg.", "/bkg/{channel}/{campaign}/[prompt+ddFakes/[data+mc/prompt]]"))
    processes.append(("Data", "/data/{channel}/{campaign}"))

    cuts = dict()
    cuts["sf"] = [
        ("2TagSR1", ["Cut2TagMllSR1VBFVeto"]),
        ("2TagSR2", ["Cut2TagMbbSR2"]),
    ]
    cuts["df"] = [("2TagSR1", ["Cut2TagMllSR1VBFVeto"])]
    cuts["ll"] = cuts["sf"]

    output_file_path = os.path.join(output_dir, "comp_mu.tex")
    with LaTeXFile.from_rel_path(output_file_path) as tex:
        tex.document_settings = ["12pt", "landscape"]
        tex.write_header()
        tex.writeline(r"\geometry{a3paper}")
        tex.begin_document()

        for campaign in campaigns:
            for channel in channels:
                logging.info("Creating cutflow for campaign '%s' and channel '%s'", campaign, channel)
                header, body = create_cutflow_table(
                    readers, campaign_to_sfpath(campaign), channel_to_sfpath(channel), processes, cuts[channel]
                )
                write_cutflow_table(tex, campaign_to_latex(campaign), channel_to_latex(channel), header, body)

        tex.end_document()
        tex.write_make_file()


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Create yield table for muon isolation wp studies")
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
