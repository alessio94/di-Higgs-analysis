#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import collections
import logging
import os

from CAFbbll.latexfile import LaTeXFile


def get_lumi_info(sample_folder):
    # first get unit conversion factor for lumi to convert to pb
    lumi_info = dict()
    lumi_info["unit"] = sample_folder.getTagStandardStringDefault("luminosityUnit", "pb")
    lumi_unit_factor = TQXSecParser.unit(TString(lumi_info["unit"]))

    # retrieve lumi values for individual campaigns
    lumi_info["c16a"] = sample_folder.getTagDoubleDefault("luminosity.c16a", 0) / lumi_unit_factor
    lumi_info["c16d"] = sample_folder.getTagDoubleDefault("luminosity.c16d", 0) / lumi_unit_factor
    lumi_info["c16e"] = sample_folder.getTagDoubleDefault("luminosity.c16e", 0) / lumi_unit_factor
    for campaign in ["c16a", "c16d", "c16e"]:
        if lumi_info[campaign] == 0:
            logging.error("Could not retrieve luminosity for campaign %s", campaign)

    # calculate total lumi
    lumi_info["run2"] = lumi_info["c16a"] + lumi_info["c16d"] + lumi_info["c16e"]
    logging.info("Lumi info: %s", lumi_info)

    return lumi_info


def read_metadata_hist(file_path):
    logging.debug("Getting metadata for file %s", file_path)
    root_file = TFile.Open(file_path)
    metadata = dict()
    try:
        metadata_hist = root_file.Get("MetaData_EventCount")
        if metadata_hist:
            metadata["nevents_initial"] = metadata_hist.GetBinContent(1)
            metadata["nevents_dxaod"] = metadata_hist.GetBinContent(7)
            metadata["nevents_cxaod"] = metadata_hist.GetBinContent(3)
            metadata["sumw_initial"] = metadata_hist.GetBinContent(4)
            metadata["sumw_dxaod"] = metadata_hist.GetBinContent(8)
            metadata["sumw_cxaod"] = metadata_hist.GetBinContent(6)
            logging.debug("Got %s", metadata)
        else:
            logging.error("Could not retrieve MetaData_EventCount histogram from file %s", file_path)
    except Exception as e:
        logging.error("An exception has occured: %s", e)
    finally:
        root_file.Close()
    return metadata


def get_metadata_info(base_samples, path):
    # for each input file read the metadata histogram
    visisted_files = set()
    samples = base_samples.getListOfSamples(path + "/?")
    metadata = collections.Counter()
    for sample in samples:
        if sample.hasTagString(".init.filepath"):
            sample_filepath = sample.getTagStandardStringDefault(".init.filepath", "")
            if not sample_filepath:
                logging.warning("Could not find tag '.init.filepath' on sample '%s'", sample.getPath().Data())
                sample.print(":td")
                continue
            if sample_filepath not in visisted_files:
                logging.debug("Getting metadata from %s", sample.getPath().Data())
                metadata.update(read_metadata_hist(sample_filepath))
                visisted_files.add(sample_filepath)
        else:
            for single_sample in sample.getListOfSamples():
                if single_sample.getPath() == sample.getPath():  # getListOfSamples() returns itself
                    continue
                sample_filepath = single_sample.getTagStandardStringDefault(".init.filepath", "")
                if not sample_filepath:
                    logging.warning(
                        "Could not find tag '.init.filepath' on sample '%s'", single_sample.getPath().Data()
                    )
                    single_sample.print(":td")
                    continue
                if sample_filepath not in visisted_files:
                    logging.debug("Getting metadata from %s", single_sample.getPath().Data())
                    metadata.update(read_metadata_hist(sample_filepath))
                    visisted_files.add(sample_filepath)
    metadata_dict = dict(metadata)

    # get cross-section info
    first_sample = samples.First()
    metadata_dict["cross_section_pb"] = first_sample.getTagDoubleDefault(".xsp.xSection", 0)
    metadata_dict["filter_eff"] = first_sample.getTagDoubleDefault(".xsp.filterEfficiency", 0)
    metadata_dict["kfactor"] = first_sample.getTagDoubleDefault(".xsp.kFactor", 0)

    logging.info("Metadata for '%s': %s", path, metadata_dict)
    return metadata_dict


def format_yield(x):
    if x >= 100:
        string_format = "{:.0f}"
    elif x >= 10:
        string_format = "{:.1f}"
    elif x >= 1:
        string_format = "{:.2f}"
    elif x >= 0.1:
        string_format = "{:.3f}"
    else:
        string_format = "{:.4f}"
    return string_format.format(x)


def write_cutflow_table(tex, columns):
    n_cols = len(columns)
    rows = list(zip(*columns))
    header = rows[0]
    body = rows[1:]

    tex.writeline(r"\begin{table}[htb]")
    tex.writeline(r"\begin{center}")
    tex.writeline(r"\resizebox{\columnwidth}{!}{%}")
    tex.writeline(r"\begin{tabular}{@{}l" + "c" * n_cols + "@{}}")
    tex.writeline(r"\toprule")

    tex.writeline(" & ".join(header) + r" \\ \midrule")
    for row in body:
        tex.writeline(" & ".join(row) + r" \\")

    tex.writeline(r"\bottomrule")
    tex.writeline(r"\end{tabular}")
    tex.writeline(r"}")
    tex.writeline(r"\end{center}")
    tex.writeline(r"\end{table}")
    tex.writeline("\n")


def main(args):
    logging.info("Loading sample folder, this may take a while ...")
    samples = TQSampleFolder.loadSampleFolder(args.input_file + ":" + args.sample_folder)
    reader = TQSampleDataReader(samples)

    output_dir = "results/cutflow_from_initial"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    processes = list()
    processes.append((r"ggF $HH\to{}bbWW$", "/sig/{channel}/{campaign}/nonres/bbWW"))
    processes.append((r"ggF $HH\to{}bb\tau\tau$", "/sig/{channel}/{campaign}/nonres/bbtautau"))
    processes.append((r"ggF $HH\to{}bb(ZZ\to{}2\ell2\nu)$", "/sig/{channel}/{campaign}/nonres/bbZZ/2l2v"))
    processes.append((r"ggF $HH\to{}bb(ZZ\to{}2\ell2q)$", "/sig/{channel}/{campaign}/nonres/bbZZ/2l2q"))
    processes.append((r"VBF $HH\to{}bbWW$", "/sig/{channel}/{campaign}/vbf_novhh_l1cvv1cv1/bbWW"))
    processes.append((r"VBF $HH\to{}bb\tau\tau$", "/sig/{channel}/{campaign}/vbf_novhh_l1cvv1cv1/bbtautau"))
    processes.append((r"VBF $HH\to{}bb(ZZ\to{}2\ell2\nu)$", "/sig/{channel}/{campaign}/vbf_novhh_l1cvv1cv1/bbZZ/2l2v"))
    processes.append((r"VBF $HH\to{}bb(ZZ\to{}2\ell2q)$", "/sig/{channel}/{campaign}/vbf_novhh_l1cvv1cv1/bbZZ/2l2q"))
    processes.append((r"res.\ $HH\to{}bbWW$ $(m_X = \SI{500}{\GeV})$", "/sig/{channel}/{campaign}/res/500/bbWW"))
    processes.append(
        (r"res.\ $HH\to{}bb\tau\tau$ $(m_X = \SI{500}{\GeV})$", "/sig/{channel}/{campaign}/res/500/bbtautau")
    )
    processes.append(
        (r"res.\ $HH\to{}bb(ZZ\to{}2\ell2\nu)$ $(m_X = \SI{500}{\GeV})$", "/sig/{channel}/{campaign}/res/500/bbZZ")
    )

    channels = "[ee+mm+me+em]"
    campaigns = "[c16a+c16d+c16e]"

    caf_cuts = list()
    caf_cuts.append((r"$N_\text{leptons} = 2$", "CutApplyZjetsSplitting"))
    caf_cuts.append(("Trigger selection and matching", "CutTriggerMatching"))
    caf_cuts.append(("Opposite sign leptons", "CutOS"))
    caf_cuts.append((r"$N_{b\text{-jets}}$ = 2 (DL1r, 77\,\%)", "CutNBJets2"))
    caf_cuts.append((r"SR1: $\SI{15}{\GeV} < m_{\ell\ell} < \SI{75 (110)}{\GeV}$ for SF (DF)", "Cut2TagMllSR1"))
    caf_cuts.append((r"SR1: VBFVeto (ggF SR)", "Cut2TagMllSR1VBFVeto"))
    caf_cuts.append((r"SR1: VBF selection (VBF SR)", "Cut2TagMllSR1VBF"))
    caf_cuts.append((r"SR2: $\SI{75}{\GeV} < m_{\ell\ell} < \SI{110}{\GeV}$, SF only", "Cut2TagMllSR2"))
    caf_cuts.append((r"SR2: $\SI{40}{\GeV} < m_{bb} < \SI{210}{\GeV}$", "Cut2TagMbbSR2"))

    lumi_info = get_lumi_info(samples)

    process_cols = list()
    description_col = list()
    first_run = True
    for process_title, process_path in processes:
        full_process_path = process_path.format(channel=channels, campaign=campaigns)
        process_metadata = get_metadata_info(samples, full_process_path)

        col = list()
        col.append(process_title)
        col.append(format_yield(process_metadata["nevents_initial"]))
        col.append(format_yield(process_metadata["sumw_initial"]))
        col.append(format_yield(lumi_info["run2"] * process_metadata["cross_section_pb"] * process_metadata["kfactor"]))
        col.append(
            format_yield(
                lumi_info["run2"]
                * process_metadata["cross_section_pb"]
                * process_metadata["kfactor"]
                * process_metadata["filter_eff"]
            )
        )
        sample_norm = (
            lumi_info["run2"]
            * process_metadata["cross_section_pb"]
            * process_metadata["kfactor"]
            * process_metadata["filter_eff"]
            / process_metadata["sumw_initial"]
        )
        col.append(format_yield(process_metadata["sumw_dxaod"] * sample_norm))
        col.append(format_yield(process_metadata["sumw_cxaod"] * sample_norm))
        if first_run:
            description_col.append("Cut")
            description_col.append("Initial number of events (raw)")
            description_col.append("Initial number of events (SumW)")
            description_col.append(r"Initial number of events ($\mathcal{L} \times \sigma$)")
            description_col.append(r"Initial number of events ($\mathcal{L} \times \sigma \times \mathcal{B}$)")
            description_col.append("SUSY2 selection")
            description_col.append("CxAOD selection")

        for caf_cut_title, caf_cut_name in caf_cuts:
            counter = reader.getCounter(full_process_path, caf_cut_name)
            if not counter:
                logging.error("Could not retrieve counter for path '%s' and cut '%s'", full_process_path, caf_cut_name)
                col.append("--")
            else:
                col.append(format_yield(counter.getCounter()))

            if first_run:
                description_col.append(caf_cut_title)

        process_cols.append(col)
        if first_run:
            first_run = False

    cols = [description_col] + process_cols

    output_file_path = os.path.join(output_dir, "cutflow.tex")
    with LaTeXFile.from_rel_path(output_file_path) as tex:
        tex.document_settings = ["12pt", "landscape"]
        tex.write_header()
        tex.writeline(r"\geometry{a3paper}")
        tex.begin_document()
        write_cutflow_table(tex, cols)
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
    from ROOT import TString, TFile
    from QFramework import TQSampleFolder, TQSampleDataReader, TQXSecParser

    main(ARGS)
