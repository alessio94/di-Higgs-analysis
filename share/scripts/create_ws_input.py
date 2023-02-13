#!/usr/bin/env python

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import collections
import itertools
import logging
import os

from ROOT import TFile


Region = collections.namedtuple("Region", "name cut channel overwrite_hist")
Observable = collections.namedtuple("Observable", "name hist")
Sample = collections.namedtuple("Sample", "name path")
Systematic = collections.namedtuple("Systematic", "name channel_suffix")


def ensure_directory(path):
    """Create a directory for the file if it is not existing yet.

    Arguments:
        path (str): Path to the file.
    """
    dir_path = os.path.dirname(path)
    if not os.path.exists(dir_path):
        os.makedirs(dir_path)


def main(args):
    regions = list()
    regions.append(Region("SR_SF", "Cut2TagMllSR1", "ee+mm", ""))
    regions.append(Region("SR_DF", "Cut2TagMllSR1", "em+me", ""))
    regions.append(Region("TopCR", "Cut2TagMllTopCR", "ee+mm+em+me", "BDT_NONRES_unblind"))

    observables = list()
    observables.append(Observable("BDT", "BDT_NONRES"))
    # observables.append(Observable("BDT_FINE", "BDT_NONRES_FINE"))

    samples = list()
    samples.append(Sample("bbtautau", "/sig/{channel}/[c16a+c16d+c16e]/nonres/bbtautau"))
    samples.append(Sample("bbww", "/sig/{channel}/[c16a+c16d+c16e]/nonres/bbWW"))
    samples.append(Sample("bbzz", "/sig/{channel}/[c16a+c16d+c16e]/nonres/bbZZ"))
    samples.append(Sample("top", "/bkg/{channel}/[c16a+c16d+c16e]/top/[ttbar/nonallhad+stop+ttbarV]"))
    samples.append(Sample("diboson", "/bkg/{channel}/[c16a+c16d+c16e]/diboson"))
    samples.append(Sample("zll", "/bkg/{channel}/[c16a+c16d+c16e]/[Zee+Zmm]"))
    samples.append(Sample("ztautau", "/bkg/{channel}/[c16a+c16d+c16e]/Ztt"))
    samples.append(Sample("fakes", "/bkg/{channel}/[c16a+c16d+c16e]/fakes"))
    samples.append(Sample("h", "/bkg/{channel}/[c16a+c16d+c16e]/Higgs"))
    samples.append(Sample("data", "/data/{channel}/[c16a+c16d+c16e]"))

    systematics = list()
    systematics.append(Systematic("nominal", ""))

    logging.info("Loading sample folder, this may take a while ...")
    sample_folder = TQSampleFolder.loadSampleFolder(args.input_file + ":" + args.sample_folder)
    reader = TQSampleDataReader(sample_folder)

    output_file_path = args.output_file
    ensure_directory(output_file_path)
    output_file = TFile.Open(output_file_path, "RECREATE")

    for region, observable, sample, systematic in itertools.product(regions, observables, samples, systematics):
        logging.debug(
            "region=%s, observable=%s, sample=%s, systematic=%s",
            region.name,
            observable.name,
            sample.name,
            systematic.name,
        )

        # build channel expression
        channel = region.channel
        if systematic.channel_suffix:
            channel = "+".join([c + "_" + systematic.channel_suffix for c in channel.split("+")])
        if "+" in channel:
            channel = "[{}]".format(channel)
        logging.debug("channel=%s", channel)

        # build histogram paths
        hist_path_in = sample.path.format(channel=channel)
        hist_path_out = "{}/{}/{}".format(region.name, observable.name, sample.name)
        hist_name_out = systematic.name

        # get histogram
        if region.overwrite_hist:
            hist_name_in = region.overwrite_hist
        else:
            hist_name_in = observable.hist
        hist = reader.getHistogram(hist_path_in, "{}/{}".format(region.cut, hist_name_in))
        if not hist:
            logging.error("Error retrieving histogram %s/%s from path '%s'!", region.cut, observable.hist, hist_path_in)
            continue

        output_file.mkdir(hist_path_out)
        output_dir_root = output_file.Get(hist_path_out)
        output_dir_root.cd()
        hist.Write(hist_name_out)


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Create a workspace input file.")
    PARSER.add_argument("input_file", metavar="INPUT_FILE", type=str, help="Input TQSampleFolder")
    PARSER.add_argument("output_file", metavar="OUTPUT_FILE", type=str, help="Output workspace input file")
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
