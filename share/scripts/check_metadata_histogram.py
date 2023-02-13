#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Check the retrieval of the initial sum-of-weights from a metadata histogram.
usage: /scripts/check_metadata_histogram.py
       [-h] [--sample-folder SAMPLE_FOLDER] [--hist-name HIST_NAME]
       [--bin BIN] [--debug]
       INPUT_FILE SAMPLE_PATH

example calls

./scripts/check_metadata_histogram.py sampleFolders/initialized/samples-initialized-bbll.root "/bkg/ee/?/top/ttbar/nonallhad"
./scripts/check_metadata_histogram.py sampleFolders/initialized/samples-initialized-bbll.root "/bkg/ee/?/top/ttbar/dilepton"
"""

from __future__ import division, print_function  # division with two integers gives float, print is a function

import argparse
import collections
import logging
import os

from ROOT import TFile


def get_sow_from_metadata_hist(path, hist_name, hist_bin):
    if not path:
        return 0.0
    root_file = TFile.Open(path.Data(), "READ")
    sow = 0.0
    try:
        hist = root_file.Get(hist_name)
        sow = hist.GetBinContent(hist_bin)
    finally:
        root_file.Close()
    return sow


def main(args):
    logging.info("Loading sample folder, this may take a while ...")
    base_samples = TQSampleFolder.loadSampleFolder(args.input_file + ":" + args.sample_folder)

    sow_per_campaign = collections.defaultdict(int)

    for sample_folder in sorted(base_samples.getListOfSampleFolders(args.sample_path)):
        logging.debug("Found TQSampleFolder %s", sample_folder.getPath())
        # get DSID level sample folders
        dsid_folders = sample_folder.getListOfSamples()
        dsid_folders_filtered = [s for s in dsid_folders if s.hasSubSamples()]
        if not dsid_folders_filtered:
            dsid_folders_filtered = dsid_folders
        # loop over contents of each DSID
        for dsid_folder in sorted(dsid_folders_filtered):
            logging.debug("Found (multi)sample %s", dsid_folder.getPath())
            campaign = dsid_folder.getTagDefault("campaign", "none").Data()
            samples = dsid_folder.getListOfSamples()
            samples = [s for s in samples if not s.hasSubSamples()]
            for sample in sorted(samples):
                logging.debug("Found sample %s", sample.getPath())

                sow_from_tag = sample.getTagDefault(".init.sumOfWeights", 0.0)
                sow_per_campaign[campaign] += sow_from_tag

                file_path = sample.getTagDefault(".init.filepath", "")
                sow_from_hist = get_sow_from_metadata_hist(file_path, args.hist_name, args.bin)

                ratio = "--"
                if sow_from_hist != 0:
                    ratio = "{:f}".format(sow_from_tag / sow_from_hist)
                logging.info("%s: %e %e %s", sample.getPath(), sow_from_tag, sow_from_hist, ratio)

    logging.info("Summary:")
    for campaign, sow in sorted(sow_per_campaign.items()):
        logging.info("  Campaign %s: Total initial sum-of-weights = %12.4f", campaign, sow)


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Check the retrieval of the initial sum-of-weights from a metadata histogram.")
    PARSER.add_argument("input_file", metavar="INPUT_FILE", type=str, help="Input TQSampleFolder")
    PARSER.add_argument(
        "sample_path", metavar="SAMPLE_PATH", type=str, help="Path to sample folder where the check should be done"
    )
    PARSER.add_argument("--sample-folder", type=str, help="Name of sample folder", default="samples")
    PARSER.add_argument("--hist-name", type=str, help="Name of metadata histogram", default="MetaData_EventCount")
    PARSER.add_argument("--bin", type=int, help="Bin of initial sum-of-weights value in metadata histogram", default=4)
    PARSER.add_argument("--debug", dest="debug", action="store_true", help="Print debug messages", default=False)
    ARGS = PARSER.parse_args()

    LOG_LEVEL = logging.DEBUG if ARGS.debug else logging.INFO
    logging.basicConfig(
        level=LOG_LEVEL, format="%(asctime)s - %(levelname)s - %(message)s", datefmt="%Y-%m-%d %H:%M:%S"
    )

    # only load QFramework after command line arguments have been parsed
    from QFramework import TQSampleFolder, TQSample

    main(ARGS)
