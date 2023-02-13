#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import logging
import math
import os

from CAFbbll.latexfile import LaTeXFile


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

    # this list contains 3-tuples with ("channel", "CutName", "HistogramName")
    hist_info = list()
    hist_info.append(("[ee+mm]", "CutMll", "BDT_NONRES"))
    hist_info.append(("[em+me]", "CutMll", "BDT_NONRES"))
    hist_info.append(("[ee+mm+em+me]", "CutMll", "BDT_NONRES"))

    output_directory = "results/binned_sig_{:f}/".format(args.femtobarn)
    ensure_directory(output_directory)
    output_file_name = os.path.splitext(os.path.basename(args.input_file))[0] + ".tex"

    scale = args.femtobarn / 36.1
    logging.info("Scaling to %.2f fb^-1", args.femtobarn)
    logging.info("scale=%f", scale)

    with LaTeXFile.from_rel_path(os.path.join(output_directory, output_file_name)) as tex:
        tex.write_header()
        tex.begin_document()

        logging.info("Getting per-bin significances")
        for channel, cut_name, histogram_name in hist_info:
            logging.info("Processing histogram %s/%s for channel %s", cut_name, histogram_name, channel)
            hist_sig = reader.getHistogram("sig/" + channel + "/nonres", "{}/{}".format(cut_name, histogram_name))
            hist_bkg = reader.getHistogram("bkg/" + channel, "{}/{}".format(cut_name, histogram_name))

            table_data = list()
            sigs = list()
            count_s = 0
            count_b = 0
            for i in range(1, hist_sig.GetNbinsX() + 1):
                s = hist_sig.GetBinContent(i) * scale
                b = hist_bkg.GetBinContent(i) * scale

                count_s += s
                count_b += b
                if b != 0:
                    z = math.sqrt(2 * ((s + b) * math.log(1 + s / b) - s))
                    sigs.append(z)
                else:
                    z = "--"
                table_data.append((i, s, b, z))
                logging.debug("Bin % 2d: s=%g, b=%g => Z=%g", i, s, b, z)
            table_data.append(("Total", count_s, count_b, math.sqrt(sum([z ** 2 for z in sigs]))))

            tex.write_table(
                table_data,
                ["{}", "{:.6f}", "{:.2f}", "{:.4f}"],
                ["Bin", "s", "b", "Significance"],
                "{} {}/{}".format(channel, cut_name, histogram_name),
                format_rows="cccc",
            )

        tex.end_document()
        tex.write_make_file()


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Get the optimized binning for the BDT distributions")
    PARSER.add_argument("input_file", metavar="INPUT_FILE", type=str, help="Input file")
    PARSER.add_argument(
        "--sample-folder", dest="sample_folder", type=str, help="Name of sample folder", default="samples"
    )
    PARSER.add_argument(
        "--scale", dest="femtobarn", type=float, help="Scale to this amout of inverse femtobarn", default=36.1
    )
    PARSER.add_argument("--debug", dest="debug", action="store_true", help="Print debug messages", default=False)

    ARGS = PARSER.parse_args()
    LOG_LEVEL = logging.DEBUG if ARGS.debug else logging.INFO
    logging.basicConfig(
        level=LOG_LEVEL, format="%(asctime)s - %(levelname)s - %(message)s", datefmt="%Y-%m-%d %H:%M:%S"
    )

    from QFramework import TQSampleFolder, TQSampleDataReader

    main(ARGS)
