#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import logging
import math
import os
import sys

from CAFbbll.latexfile import LaTeXFile
from CAFbbll.pdg_rounding import pdgRound


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


def create_cutflow_table(reader, campaign, channel, processes, cuts_dict, category):
    header = (
        ["Cut"]
        + list(zip(*processes)[0])
        + [
            r"$\epsilon_{s}$",
            r"$\epsilon_\text{Top}$",
            r"$\epsilon_\text{ZHF}$",
            r"$\epsilon_\text{Fakes}$",
            r"$\epsilon_\text{Other}$",
            r"$\epsilon_\text{Bkg.}$",
            r"$\epsilon_\text{Data}$",
            "other frac.",
            "s/b",
            "Z",
        ]
    )
    body = list()
    ref_sig_count = 0
    ref_top_count = 0
    ref_zhf_count = 0
    ref_fakes_count = 0
    ref_other_count = 0
    ref_bkg_count = 0
    ref_data_count = 0

    for cat_title, cat_names in category:
        if not (cat_title.find("SR2") == -1) and channel == channel_to_sfpath("df"):
            continue
        body.append(cat_title)

        cuts = cuts_dict[cat_names[0]]
        for n_cut, (cut_title, cut_name) in enumerate(cuts):
            if cut_title == "|":
                body.append("|")
                continue

            row = list()
            row.append(cut_title)

            sig_count = 0
            top_count = 0
            zhf_count = 0
            fakes_count = 0
            other_count = 0
            bkg_count = 0
            data_count = 0

            for process_name, process_path in processes:
                process_path_formatted = process_path.format(campaign=campaign, channel=channel)
                for n_name, cat_name in enumerate(cat_names):
                    if n_name == 0:
                        counter = reader.getCounter(process_path_formatted, cut_name)
                    else:
                        counter.add(reader.getCounter(process_path_formatted, cuts_dict[cat_name][n_cut][1]))
                if not counter:
                    logging.error(
                        "Could not get counter for process '%s' and cut '%s'", process_path_formatted, cut_name
                    )
                    sys.exit(1)

                if process_name == "Data":
                    row.append(r"{}".format(int(counter.getCounter())))
                else:
                    row.append(r"{} $\pm$ {}".format(*pdgRound(counter.getCounter(), counter.getError())))

                if process_name == "Signal":
                    sig_count = counter.getCounter()
                    if cut_name == cuts[0][1]:
                        ref_sig_count = sig_count
                elif process_name == "Top":
                    top_count = counter.getCounter()
                    if cut_name == cuts[0][1]:
                        ref_top_count = top_count
                elif process_name == "Z+HF":
                    zhf_count = counter.getCounter()
                    if cut_name == cuts[0][1]:
                        ref_zhf_count = zhf_count
                elif process_name == "Fakes":
                    fakes_count = counter.getCounter()
                    if cut_name == cuts[0][1]:
                        ref_fakes_count = fakes_count
                elif process_name == "Other bkg.":
                    other_count = counter.getCounter()
                    if cut_name == cuts[0][1]:
                        ref_other_count = other_count
                elif process_name == "Bkg.":
                    bkg_count = counter.getCounter()
                    if cut_name == cuts[0][1]:
                        ref_bkg_count = bkg_count
                elif process_name == "Data" and "NN" not in cut_name:
                    data_count = counter.getCounter()
                    if cut_name == cuts[0][1]:
                        ref_data_count = data_count

            def calc_eff(count, ref_count):
                if ref_count > 0:
                    return "{:.2f}".format(count / ref_count)
                return "--"

            eff_sig = calc_eff(sig_count, ref_sig_count)
            eff_top = calc_eff(top_count, ref_top_count)
            eff_zhf = calc_eff(zhf_count, ref_zhf_count)
            eff_fakes = calc_eff(fakes_count, ref_fakes_count)
            eff_other = calc_eff(other_count, ref_other_count)
            eff_bkg = calc_eff(bkg_count, ref_bkg_count)
            eff_data = calc_eff(data_count, ref_data_count)
            row.append(eff_sig)
            row.append(eff_top)
            row.append(eff_zhf)
            row.append(eff_fakes)
            row.append(eff_other)
            row.append(eff_bkg)
            row.append(eff_data)

            if bkg_count > 0:
                other_fraction = "{:.4f}".format(other_count / bkg_count)
                s_over_b = "{:.7f}".format(sig_count / bkg_count)
            else:
                other_fraction = "--"
                s_over_b = "--"
            row.append(other_fraction)
            row.append(s_over_b)
            row.append("{:.5f}".format(asimov_sig(sig_count, bkg_count)))

            body.append(row)

        body.append("||")

    return header, body[0:-1]


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
        elif row == "||":
            tex.writeline(r"\midrule\midrule")
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
    logging.info("Loading sample folder, this may take a while ...")
    samples = TQSampleFolder.loadSampleFolder(args.input_file + ":" + args.sample_folder)
    reader = TQSampleDataReader(samples)

    output_dir = "results/btag/btagging/"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    channels = ["sf", "df", "ll"]
    campaigns = ["run2"]

    process_top = "/bkg/{channel}/{campaign}/prompt/top/[ttbar+stop/Wt+ttbarV]"
    process_zhf = "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm+Ztt]/hf"
    process_fakes = "/bkg/{channel}/{campaign}/ddFakes/[data+mc/prompt]"
    process_other = "/bkg/{channel}/{campaign}/prompt/[[Zee+Zmm+Ztt]/lf+diboson+Higgs+top/stop/[schan+tchan]+Wjets]"
    processes = list()
    processes.append(("Signal", "/sig/{channel}/{campaign}/nonres"))
    processes.append(("Top", process_top))
    processes.append(("Z+HF", process_zhf))
    processes.append(("Fakes", process_fakes))
    processes.append(("Other bkg.", process_other))
    processes.append(("Bkg.", "bkg/{channel}/{campaign}/[prompt+ddFakes/[data+mc/prompt]]"))
    processes.append(("Data", "/data/{channel}/{campaign}"))

    dir_prefix = ["Cut2TagMllSR1", "Cut2TagMbbSR2"]
    cuts = dict()
    for prefix in dir_prefix:
        # title = (prefix.replace('Cut','')).replace('Mll','')

        cut = list()
        cut.append((r"DL1r 70\,\%", prefix + "_" + "DL1r_FixedCutBEff70"))
        cut.append((r"DL1r 77\,\%", prefix + "_" + "DL1r_FixedCutBEff77"))
        cut.append((r"DL1r 85\,\%", prefix + "_" + "DL1r_FixedCutBEff85"))
        cuts[prefix] = cut

    category = [
        ("2TagSR1", ["Cut2TagMllSR1"]),
        ("2TagSR2", ["Cut2TagMbbSR2"]),
    ]
    output_file_path = os.path.join(output_dir, "tag_comp_btag.tex")
    with LaTeXFile.from_rel_path(output_file_path) as tex:
        tex.document_settings = ["12pt", "landscape"]
        tex.write_header()
        tex.writeline(r"\geometry{a3paper}")
        tex.begin_document()
        for campaign in campaigns:
            for channel in channels:
                logging.info("Creating cutflow for campaign '%s' and channel '%s'", campaign, channel)
                header, body = create_cutflow_table(
                    reader, campaign_to_sfpath(campaign), channel_to_sfpath(channel), processes, cuts, category
                )
                write_cutflow_table(tex, campaign_to_latex(campaign), channel_to_latex(channel), header, body)

        tex.end_document()
        tex.write_make_file()

    category = [("SR1", ["Cut2TagMllSR1"]), ("SR2", ["Cut2TagMbbSR2"])]
    output_file_path = os.path.join(output_dir, "sr_comp_btag.tex")
    with LaTeXFile.from_rel_path(output_file_path) as tex:
        tex.document_settings = ["12pt", "landscape"]
        tex.write_header()
        tex.writeline(r"\geometry{a3paper}")
        tex.begin_document()
        for campaign in campaigns:
            for channel in channels:
                logging.info("Creating cutflow for campaign '%s' and channel '%s'", campaign, channel)
                header, body = create_cutflow_table(
                    reader, campaign_to_sfpath(campaign), channel_to_sfpath(channel), processes, cuts, category
                )
                write_cutflow_table(tex, campaign_to_latex(campaign), channel_to_latex(channel), header, body)

        tex.end_document()
        tex.write_make_file()


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Create yield table for b-tagging studies")
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
