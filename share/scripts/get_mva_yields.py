#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import collections
import logging
import math
import os

from CAFbbll.latexfile import LaTeXFile


Process = collections.namedtuple("Process", "name latex path")


def ensure_directory(path):
    """Create a direcotry.

    Arguments:
        path (str): Path of the directory to create.
    """
    if not os.path.exists(path):
        os.makedirs(path)


def main(args):
    """Entry point of this script."""
    samples = TQSampleFolder.loadLazySampleFolder(args.input_file + ":" + args.sample_folder)
    reader = TQSampleDataReader(samples)

    # this list contains 2-tuples with ("CutName", "HistogramName")
    hist_info = list()
    hist_info.append(("Cut2TagMllSR1VBFVeto", "NN_SR1_Signal_Rebin", "[ee+mm+em+me]"))
    hist_info.append(("Cut2TagMllSR1VBFVeto", "NN_SR1_Top", "[ee+mm+em+me]"))
    hist_info.append(("Cut2TagMllSR1VBFVeto", "NN_SR1_Other", "[ee+mm+em+me]"))

    processes = list()
    processes.append(Process("sig", r"Signal", "/sig/{channel}/{campaign}/nonres"))
    processes.append(Process("bkg", r"Background", "/bkg/{channel}/{campaign}/[prompt+nonprompt]"))

    output_directory = "results/mva_yields_soverb/"
    ensure_directory(output_directory)
    output_file_name = os.path.splitext(os.path.basename(args.input_file))[0] + ".tex"

    with LaTeXFile.from_rel_path(os.path.join(output_directory, output_file_name)) as tex:
        tex.document_settings.append("landscape")
        tex.write_header()
        tex.begin_document()

        logging.info("Getting per-bin significances")
        for cut_name, histogram_name, channel in hist_info:
            logging.info("Processing %s/%s", cut_name, histogram_name)
            hists = dict()
            for process in processes:
                campaign = "[c16a+c16d+c16e]"
                hists[process.name] = reader.getHistogram(
                    process.path.format(channel=channel, campaign=campaign), "{}/{}".format(cut_name, histogram_name)
                )

            table_data = list()
            sigs = list()
            hist_sig = hists["sig"]
            hist_bkg = hists["bkg"]
            for i in range(1, hist_sig.GetNbinsX() + 1):
                s = hist_sig.GetBinContent(i)
                b = hist_bkg.GetBinContent(i)

                if b != 0:
                    # z = math.sqrt(2 * ((s + b) * math.log(1 + s / b) - s))
                    z = s / math.sqrt(b)
                    sigs.append(z)
                else:
                    z = "--"
                table_data.append((i, z))
                logging.debug("Bin % 2d: %g", i, z)
            table_data.append(("Total", math.sqrt(sum([z ** 2 for z in sigs]))))

            tex.write_table(
                table_data,
                ["{}", "{:.4f}"],
                ["Bin", "Significance"],
                "{}/{}".format(cut_name, histogram_name),
                format_rows="cc",
            )

        tex.end_document()
        tex.write_make_file()


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Get the optimized binning for the BDT distributions")
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

    from QFramework import TQSampleFolder, TQSampleDataReader

    main(ARGS)
