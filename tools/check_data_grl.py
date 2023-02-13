#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Check if the data in a TQSampleFolder is consistent with a given GRL.

Usage
=====

To run the check for 15+16 GRL:
./check_data_grl.py path/to/tqsamplefolder

You can specify the years with the --years option (e.g. for 2015-2017):
./check_data_grl.py path/to/tqsamplefolder --years 151617

Or if you want to check for full Run-2 data
./check_data_grl.py path/to/tqsamplefolder --years all


Settings
========

To check if all runs from the sample folder are in the GRL the run number needs
be extracted from the sample folder path. This is done with a regular expression,
which is set in the variable 'SAMPLE_FOLDER_REGEX'. You may need to adjust this
for your analysis.

The paths to the GRLs are set in the 'GRLS' dictionary. If a new GRL is released
this should be updated.


Arguments
=========

./check_data_grl.py [-h] [--sample-folder SAMPLE_FOLDER] [--data-folder DATA_FOLDER]
       [--years YEARS] [--debug]
       INPUT_FILE

positional arguments:
  INPUT_FILE            Input file

optional arguments:
  -h, --help            show this help message and exit
  --sample-folder SAMPLE_FOLDER
                        Name of sample folder (default=samples)
  --data-folder DATA_FOLDER
                        Path of data folder (default=/data/?/all)
  --years YEARS         Years to check (e.g. "151617" or "all", default=1516)
  --debug               Print debug messages

"""

import argparse
import logging
import re
import sys
import xml.etree.ElementTree


SAMPLE_FOLDER_REGEX = r".*data_(\d+)\..*"

GRLS = dict()
GRLS[
    "15"
] = "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/GoodRunsLists/data15_13TeV/20170619/physics_25ns_21.0.19.xml"
GRLS[
    "16"
] = "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/GoodRunsLists/data16_13TeV/20180129/physics_25ns_21.0.19.xml"
GRLS[
    "17"
] = "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/GoodRunsLists/data17_13TeV/20180619/physics_25ns_Triggerno17e33prim.xml"
GRLS[
    "18"
] = "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/GoodRunsLists/data18_13TeV/20181111/physics_25ns_Triggerno17e33prim.xml"


def load_data_samples(input_file, sample_folder, data_folder):
    """Load the data sample folders from the given file path.

    Arguments:
        input_file (str):    Path to the TQSampleFolder file.
        sample_folder (str): Name of the TQSampleFolder.
        data_folder (str):   Path to the data folder.

    Returns:
        list(str): A list of all data sample folder paths.
    """
    logging.info("Loading sample folder %s:%s", input_file, sample_folder)
    samples = TQSampleFolder.loadLazySampleFolder(input_file + ":" + sample_folder)
    data_folder = data_folder.rstrip("/")
    data_path = "{}/*".format(data_folder)
    data_samples = [str(s) for s in samples.getSampleFolderPathsWildcarded(data_path)]
    data_samples.remove(data_folder + "/")
    if len(data_samples) == 0:
        logging.warning("Could not find any sample in '{}'".format(data_path))
    return data_samples


def parse_grl(path, year):
    """Parse a GRL for all the runs it contains.

    Arguments:
        path (str): Full path to the GRL xml file.
        year (str): Two-digit string indicating the year of the GRL.

    Returns:
        list((str, str)): A list of tuples, where the first entry is
                          the run number and the second element is the
                          year of this run number.
    """
    logging.debug("Parsing GRL for year 20%s: %s", year, path)
    tree = xml.etree.ElementTree.parse(path)
    root = tree.getroot()

    # find the run list  metadata
    all_runs = None
    for metadata in root.iter("Metadata"):
        if metadata.attrib["Name"] == "RunList":
            all_runs = metadata.text

    if all_runs is None:
        logging.warning("Could not parse GRL '%s'", path)
        return list()

    run_list = all_runs.split(",")
    return [(run, year) for run in run_list]


def load_runs_from_grls(years):
    """Load all the runs from the GRL files for the given years.

    Arguments:
        years (str): A string containing two-digit numbers indicating
                     the years for which the runs are requested.
                     If the argument is "all" the full Run-2 dataset
                     is used.

    Returns:
        list((str, str)): A list of tuples, where the first entry is
                          the run number and the second element is the
                          year of this run number.
    """
    if years == "all":
        years = "15161718"
    logging.debug("Loading GRLs for the years '%s'", years)

    runs = list()
    for possible_year in sorted(GRLS.keys()):
        if possible_year in years:
            current_runs = parse_grl(GRLS[possible_year], possible_year)
            logging.info("Found %3d runs in GRL for year 20%s", len(current_runs), possible_year)
            runs += current_runs

    return runs


def check_runs_in_samples(runs, samples):
    """Check the consistency between the runs in the GRLs and the TQSampleFolder.

    Arguments:
        runs (list((str, str))): A list of tuples, where the first entry is
                                 the run number and the second element is the
                                 year of this run number.

        samples (list(str)):     A list of all data sample folder paths.

    Returns:
        list((str, str)), list(str): Two lists, the first one contains the runs (+ year)
                                     which are missing in the TQSampleFolder, the second
                                     one contains runs which are in the TQSampleFolder
                                     but not in the GRL.
    """
    missing_in_samples = list()
    missing_in_grl = list()

    # check for runs which are in GRL but not in sample folder
    for run, year in runs:
        found = False
        for sample in samples:
            if run in sample:
                found = True
                break
        if not found:
            missing_in_samples.append((run, year))
            logging.debug(
                "Run %s is missing in sample folder but is existing in the GRL (this is for year 20%s)", run, year
            )

    # check for runs which are in sample foler but in GRL
    sample_regex = re.compile(SAMPLE_FOLDER_REGEX)
    runs_without_years = list(zip(*runs))[0]
    for sample in samples:
        match = sample_regex.match(sample)
        if match:
            if len(match.groups()) != 1:
                logging.warning("The regex has to have exactly one group which matches the run number!")
                break
            else:
                run_number = str(int(match.group(1)))  # remove leading zeros with str(int(.)) conversion
                if not run_number in runs_without_years:
                    missing_in_grl.append(run_number)
                    logging.debug("Run %s is existing in the sample folder but missing in the GRL", run_number)
        else:
            logging.info("Could not match sample folder path '%s' with given regex.", sample)

    return missing_in_samples, missing_in_grl


def main(args):
    """Main entry point of this script."""
    data_samples = load_data_samples(args.input_file, args.sample_folder, args.data_folder)
    runs = load_runs_from_grls(args.years)

    if args.debug:
        logging.debug("The following samples were found in the sample folder:")
        for data_sample in data_samples:
            logging.debug(data_sample)
        logging.debug("")
        logging.debug("The following runs were found in the GRLs:")
        for run in runs:
            logging.debug(run)

    missing_in_samples, missing_in_grl = check_runs_in_samples(runs, data_samples)
    print("\nResults")
    print("=======")
    print("There are {:d} run(s) missing in the sample folder:".format(len(missing_in_samples)))
    for run, year in missing_in_samples:
        print("Run {:s}, year 20{:s}".format(run, year))
    print("")
    print(
        "There are {:d} run(s) in the sample folder but not in the GRL (this is ok if the files are empty, but you may want to remove those files):".format(
            len(missing_in_grl)
        )
    )
    for run in missing_in_grl:
        print("Run {:s}".format(run))


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Check if the data in a TQSampleFolder is consistent with a given GRL")
    PARSER.add_argument("input_file", metavar="INPUT_FILE", type=str, help="Input file")
    PARSER.add_argument(
        "--sample-folder",
        dest="sample_folder",
        type=str,
        help="Name of sample folder (default=samples)",
        default="samples",
    )
    PARSER.add_argument(
        "--data-folder",
        dest="data_folder",
        type=str,
        help="Path of data folder (default=/data/?/all)",
        default="/data/?/all",
    )
    PARSER.add_argument(
        "--years", dest="years", type=str, help='Years to check (e.g. "151617" or "all", default=1516)', default="1516"
    )
    PARSER.add_argument("--debug", dest="debug", action="store_true", help="Print debug messages", default=False)

    ARGS = PARSER.parse_args()
    LOG_LEVEL = logging.DEBUG if ARGS.debug else logging.INFO
    logging.basicConfig(
        level=LOG_LEVEL, format="%(asctime)s - %(levelname)s - %(message)s", datefmt="%Y-%m-%d %H:%M:%S"
    )

    from QFramework import TQSampleFolder

    main(ARGS)
