#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import logging
import math
import os

from CAFbbll.textfile import TextFile
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


def write_make_file(output_dir):
    output_file_path = os.path.join(output_dir, "Makefile")
    with TextFile(output_file_path, "w") as makefile:
        makefile.writeline("SOURCES = $(wildcard *.tex)")
        makefile.writeline("TARGETS = $(SOURCES:%.tex=%.pdf)")
        makefile.writeline("")
        makefile.writeline(".PHONY: default")
        makefile.writeline("default: $(TARGETS)")
        makefile.writeline("")
        makefile.writeline("$(TARGETS): %.pdf : %.tex")
        makefile.writeline("\tlatexmk -f -pdf $<")


def create_yield_table(reader, campaign, channel, processes, cut, distribution, rounding):
    header = None
    # header = ["Process"] + list(zip(*cuts)[0])
    body = list()
    for process_name, process_path in processes:
        row = list()
        row.append(process_name)

        full_process_path = process_path.format(campaign=campaign, channel=channel)
        hist = reader.getHistogram(full_process_path, "{}/{}".format(cut, distribution))

        if not hist:
            logging.error("Could not retrieve histogram '%s/%s' for process '%s'", cut, distribution, full_process_path)
            continue

        if not header:
            header = ["Process"] + ["Bin {}".format(ibin) for ibin in range(1, hist.GetNbinsX() + 1)]

        for ibin in range(1, hist.GetNbinsX() + 1):
            if rounding:
                row.append(r"{} $\pm$ {}".format(*pdgRound(hist.GetBinContent(ibin), hist.GetBinError(ibin))))
            else:
                row.append(r"{} $\pm$ {}".format(hist.GetBinContent(ibin), hist.GetBinError(ibin)))

        body.append(row)

    return header, body


def write_cutflow_table(tex, header, body):
    n_cols = len(header)

    tex.writeline("\n\n")
    tex.writeline(r"\begin{table}[htb]")
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
    tex.writeline("\n\n")


def main(args):
    logging.info("Loading sample folder, this may take a while ...")
    samples = TQSampleFolder.loadSampleFolder(args.input_file + ":" + args.sample_folder)
    reader = TQSampleDataReader(samples)

    channels = ["ll"]
    campaigns = ["run2"]

    processes = list()
    processes.append((r"Signal", "/sig/{channel}/{campaign}/[nonres+vbf_novhh_l1cvv1cv1]"))
    processes.append((r"HH non.\ res. ggF", "/sig/{channel}/{campaign}/nonres"))
    processes.append((r"HH non.\ res. VBF", "/sig/{channel}/{campaign}/vbf_novhh_l1cvv1cv1"))
    processes.append((r"HH non.\ res. ggF $bbWW$", "/sig/{channel}/{campaign}/nonres/bbWW"))
    processes.append((r"HH non.\ res. ggF $bb\tau\tau$", "/sig/{channel}/{campaign}/nonres/bbtautau"))
    processes.append((r"HH non.\ res. ggF $bbZZ$", "/sig/{channel}/{campaign}/nonres/bbZZ"))
    processes.append((r"Total background (MC Fakes)", "/bkg/{channel}/{campaign}/[prompt+nonprompt]"))
    processes.append((r"Total background (SSOS Fakes)", "/bkg/{channel}/{campaign}/[prompt+ddFakes/[data+mc/prompt]]"))
    processes.append((r"$t\overline{t}$ (dilep)", "/bkg/{channel}/{campaign}/prompt/top/ttbar/dilepton"))
    processes.append((r"$t\overline{t}$ (lephad)", "/bkg/{channel}/{campaign}/prompt/top/ttbar/nonallhad"))
    processes.append((r"$t\overline{t}$ (hadhad)", "/bkg/{channel}/{campaign}/prompt/top/ttbar/allhad"))
    processes.append((r"single top ($Wt)$", "/bkg/{channel}/{campaign}/prompt/top/stop/Wt"))
    processes.append((r"single top (s-channel)", "/bkg/{channel}/{campaign}/prompt/top/stop/schan"))
    processes.append((r"single top (t-channel)", "/bkg/{channel}/{campaign}/prompt/top/stop/tchan"))
    processes.append((r"$t\overline{t}+V$", "/bkg/{channel}/{campaign}/prompt/top/ttbarV"))
    processes.append((r"$Z\to{}\ell\ell$ (HF, HM)", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/hf/main"))
    processes.append((r"$Z\to{}\ell\ell$ (HF, LM)", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/hf/lowmass"))
    processes.append((r"$Z\to{}\ell\ell$ (HF, VLM)", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/hf/verylowmass"))
    processes.append((r"$Z\to{}\ell\ell$ (HF, EWK)", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/hf/ew"))
    processes.append((r"$Z\to{}\ell\ell$ (LW, HM)", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/lf/main"))
    processes.append((r"$Z\to{}\ell\ell$ (LW, LM)", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/lf/lowmass"))
    processes.append((r"$Z\to{}\ell\ell$ (LW, VLM)", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/lf/verylowmass"))
    processes.append((r"$Z\to{}\ell\ell$ (LW, EWK)", "/bkg/{channel}/{campaign}/prompt/[Zee+Zmm]/lf/ew"))
    processes.append((r"$Z\to{}\tau\tau$ (HF)", "/bkg/{channel}/{campaign}/prompt/Ztt/hf"))
    processes.append((r"$Z\to{}\tau\tau$ (LF)", "/bkg/{channel}/{campaign}/prompt/Ztt/lf"))
    processes.append((r"Diboson", "/bkg/{channel}/{campaign}/prompt/diboson"))
    processes.append((r"W+jets", "/bkg/{channel}/{campaign}/prompt/Wjets"))
    processes.append((r"Higgs", "/bkg/{channel}/{campaign}/prompt/Higgs"))
    processes.append((r"MC Fakes", "/bkg/{channel}/{campaign}/nonprompt/"))
    processes.append((r"MC Fakes ($t\overline{t}$ (dilep))", "/bkg/{channel}/{campaign}/nonprompt/top/ttbar/dilepton"))
    processes.append(
        (r"MC Fakes ($t\overline{t}$ (lephad))", "/bkg/{channel}/{campaign}/nonprompt/top/ttbar/nonallhad")
    )
    processes.append((r"MC Fakes ($t\overline{t}$ (hadhad))", "/bkg/{channel}/{campaign}/nonprompt/top/ttbar/allhad"))
    processes.append((r"MC Fakes (single top ($Wt)$)", "/bkg/{channel}/{campaign}/nonprompt/top/stop/Wt"))
    processes.append((r"MC Fakes (single top (s-channel))", "/bkg/{channel}/{campaign}/nonprompt/top/stop/schan"))
    processes.append((r"MC Fakes (single top (t-channel))", "/bkg/{channel}/{campaign}/nonprompt/top/stop/tchan"))
    processes.append((r"MC Fakes ($t\overline{t}+V$)", "/bkg/{channel}/{campaign}/nonprompt/top/ttbarV"))
    processes.append((r"MC Fakes ($Z\to{}\ell\ell$ (HF))", "/bkg/{channel}/{campaign}/nonprompt/[Zee+Zmm]/hf"))
    processes.append((r"MC Fakes ($Z\to{}\ell\ell$ (LF))", "/bkg/{channel}/{campaign}/nonprompt/[Zee+Zmm]/lf"))
    processes.append((r"MC Fakes ($Z\to{}\tau\tau$ (HF))", "/bkg/{channel}/{campaign}/nonprompt/Ztt/hf"))
    processes.append((r"MC Fakes ($Z\to{}\tau\tau$ (LF))", "/bkg/{channel}/{campaign}/nonprompt/Ztt/lf"))
    processes.append((r"MC Fakes (Diboson)", "/bkg/{channel}/{campaign}/nonprompt/diboson"))
    processes.append((r"MC Fakes (W+jets)", "/bkg/{channel}/{campaign}/nonprompt/Wjets"))
    processes.append((r"MC Fakes (Higgs)", "/bkg/{channel}/{campaign}/nonprompt/Higgs"))
    processes.append((r"SSOS Fakes", "/bkg/{channel}/{campaign}/ddFakes/[data+mc/prompt]"))
    processes.append((r"SSOS Fakes (data)", "/bkg/{channel}/{campaign}/ddFakes/data"))
    processes.append((r"SSOS Fakes (MC)", "/bkg/{channel}/{campaign}/ddFakes/mc/prompt"))
    processes.append(
        (r"SSOS Fakes ($t\overline{t}$ (dilep))", "/bkg/{channel}/{campaign}/ddFakes/mc/prompt/top/ttbar/dilepton")
    )
    processes.append(
        (r"SSOS Fakes ($t\overline{t}$ (lephad))", "/bkg/{channel}/{campaign}/ddFakes/mc/prompt/top/ttbar/nonallhad")
    )
    processes.append(
        (r"SSOS Fakes ($t\overline{t}$ (hadhad))", "/bkg/{channel}/{campaign}/ddFakes/mc/prompt/top/ttbar/allhad")
    )
    processes.append((r"SSOS Fakes (single top ($Wt)$)", "/bkg/{channel}/{campaign}/ddFakes/mc/prompt/top/stop/Wt"))
    processes.append(
        (r"SSOS Fakes (single top (s-channel))", "/bkg/{channel}/{campaign}/ddFakes/mc/prompt/top/stop/schan")
    )
    processes.append(
        (r"SSOS Fakes (single top (t-channel))", "/bkg/{channel}/{campaign}/ddFakes/mc/prompt/top/stop/tchan")
    )
    processes.append((r"SSOS Fakes ($t\overline{t}+V$)", "/bkg/{channel}/{campaign}/ddFakes/mc/prompt/top/ttbarV"))
    processes.append(
        (r"SSOS Fakes ($Z\to{}\ell\ell$ (HF))", "/bkg/{channel}/{campaign}/ddFakes/mc/prompt/[Zee+Zmm]/hf")
    )
    processes.append(
        (r"SSOS Fakes ($Z\to{}\ell\ell$ (LF))", "/bkg/{channel}/{campaign}/ddFakes/mc/prompt/[Zee+Zmm]/lf")
    )
    processes.append((r"SSOS Fakes ($Z\to{}\tau\tau$ (HF))", "/bkg/{channel}/{campaign}/ddFakes/mc/prompt/Ztt/hf"))
    processes.append((r"SSOS Fakes ($Z\to{}\tau\tau$ (LF))", "/bkg/{channel}/{campaign}/ddFakes/mc/prompt/Ztt/lf"))
    processes.append((r"SSOS Fakes (Diboson)", "/bkg/{channel}/{campaign}/ddFakes/mc/prompt/diboson"))
    processes.append((r"SSOS Fakes (W+jets)", "/bkg/{channel}/{campaign}/ddFakes/mc/prompt/Wjets"))
    processes.append((r"SSOS Fakes (Higgs)", "/bkg/{channel}/{campaign}/ddFakes/mc/prompt/Higgs"))

    distributions = list()
    distributions.append(("Cut2TagMllSR1VBFVeto", "NN_SR1_Signal_Rebin"))
    distributions.append(("Cut2TagMbbSR2", "NN_SR2_Signal_Rebin"))
    distributions.append(("Cut2TagMBLTTbarCR", "Yield"))
    distributions.append(("Cut2TagMBLWtCR", "Yield"))
    distributions.append(("Cut2TagMbbZllCR", "Yield"))

    output_dir = "results/distribution_yields"
    if not args.rounding:
        output_dir += "_norounding"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    for campaign in campaigns:
        for channel in channels:
            for cut, distribution in distributions:
                logging.info(
                    "Creating yield table for '%s/%s' campaign '%s' channel '%s'", cut, distribution, campaign, channel
                )
                header, body = create_yield_table(
                    reader,
                    campaign_to_sfpath(campaign),
                    channel_to_sfpath(channel),
                    processes,
                    cut,
                    distribution,
                    args.rounding,
                )
                output_file_name = "{}-{}-{}-{}.tex".format(campaign, channel, cut, distribution)
                output_file_path = os.path.join(output_dir, output_file_name)
                with LaTeXFile.from_rel_path(output_file_path) as tex:
                    tex.document_settings = ["12pt", "landscape"]
                    tex.write_header()
                    tex.writeline(r"\geometry{a3paper}")
                    tex.begin_document()
                    write_cutflow_table(tex, header, body)
                    tex.end_document()

    write_make_file(output_dir)


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser(
        description="Get a yields for a distribution split into different processes and bins."
    )
    PARSER.add_argument("input_file", metavar="INPUT_FILE", type=str, help="Input file")
    PARSER.add_argument(
        "--sample-folder", dest="sample_folder", type=str, help="Name of sample folder", default="samples"
    )
    PARSER.add_argument(
        "--disable-rounding", dest="rounding", action="store_false", help="Disable rounding of values", default=True
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
