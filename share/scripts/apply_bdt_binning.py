#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import array
import logging

from ROOT import TList, TString, TH1F


def remap_bdt_output(hist):
    """Remap the bins of the BDT output to equidistant bins between -1 and 1.

    Arguments:
        hist (TH1F): Histogram to remap.

    Returns:
        TH1F: New histogram with remapping.
    """
    # number of bins in the histogram
    n_bins = hist.GetNbinsX()

    # create new histogram
    remapped_hist = TH1F(hist.GetName() + "_remapped", "Remapped " + hist.GetTitle(), n_bins, -1, 1)

    # initialize structure to store weights
    remapped_hist.Sumw2()

    remapped_hist.SetDirectory(hist.GetDirectory())

    # update the x-axis title
    remapped_hist.GetXaxis().SetTitle("Remapped " + hist.GetXaxis().GetTitle())

    # copy the histogram style
    TQHistogramUtils.copyStyle(remapped_hist, hist)

    # copy the bin contents and errors
    for i in range(0, n_bins + 2):  # include under and overflow bin, loop goes from 0 to nbins + 1
        remapped_hist.SetBinContent(i, hist.GetBinContent(i))
        remapped_hist.SetBinError(i, hist.GetBinError(i))

    # return the new histogram
    return remapped_hist


def rebin_bdt_output(samples, source, binning):
    """
    Rebin (and remap) a histogram in the TQSampleFolder.

    This functions loops over all contents of the TQSampleFolder and
    applies the rebinning and remapping modifications for each sample folder.

    Arguments:
        samples (TQSampleFolder): Sample folder with histograms.
        source (string): Path of the histogram.
        binning (list(float)): List with new bin edges.
    """
    # the number of new histogram contributions
    n_histos = 0

    # get the source histogram and the list of contributing sample folders
    sf_list = TList()
    samples.getHistogram(".", source, "", sf_list)

    # the name of the rebinned histogram is the old name, but "FINE" replaced with "REBIN"
    destination = TString(source.replace("FINE", "REBIN"))
    new_name = TQFolder.getPathTail(destination)  # destination becomes everything before last /
    new_name_remap = TString(new_name).ReplaceAll("REBIN", "REMAP")
    logging.debug("New histogram names are %s and %s", new_name, new_name_remap)

    # a list to keep track of sample folders that have already been handled
    done = TList()

    for sf in sf_list:
        # skip sample folders that have already been handled
        if done.FindObject(sf):
            logging.debug("Skipping %s, already handled", sf.getName())
            continue
        done.Add(sf)

        # get the individual source histogram contribution
        hist = sf.getHistogram(".", source, "")
        if not hist:
            # we failed for some reason to obtain it
            logging.warning("Failed to obtain histogram %s in sample folder %s", source, sf.getName())
            continue

        # rebin histogram
        hist = hist.Rebin(len(binning) - 1, "", array.array("d", binning))

        # remap histogram
        hist_remap = remap_bdt_output(hist)

        # store as new histograms
        success_rebin = sf.addObject(hist, TQFolder.concatPaths(".histograms", destination) + "+::" + new_name)
        success_remap = sf.addObject(
            hist_remap, TQFolder.concatPaths(".histograms", destination) + "+::" + new_name_remap
        )
        if success_rebin and success_remap:
            n_histos += 1
        else:
            logging.warning("Failed to merge histogram %s in sample folder %s", source, sf.getName())

    logging.info("Merged and remapped %d histograms for %s", n_histos, source)


def main(args):
    """Entry point of this script."""
    hist_info = list()
    hist_info.append(
        (
            "CutMll/BDT_NONRES_SF_FINE",
            [
                -1,
                -0.906,
                -0.81,
                -0.704,
                -0.59,
                -0.435,
                -0.242,
                -0.075,
                0.07,
                0.196,
                0.305,
                0.4,
                0.482,
                0.554,
                0.619,
                0.678,
                0.73,
                0.775,
                0.813,
                0.847,
                0.877,
                0.903,
                0.926,
                0.946,
                1,
            ],
        )
    )
    hist_info.append(
        (
            "CutMll/BDT_NONRES_DF_FINE",
            [
                -1.0,
                -0.932,
                -0.82,
                -0.635,
                -0.343,
                -0.119,
                0.074,
                0.224,
                0.333,
                0.414,
                0.484,
                0.546,
                0.602,
                0.657,
                0.704,
                0.744,
                0.78,
                0.824,
                0.86,
                0.891,
                0.918,
                0.939,
                0.953,
                0.975,
                1,
            ],
        )
    )

    logging.info("Loading sample folder, this may take a while ...")
    samples = TQSampleFolder.loadSampleFolder(args.input_file + ":" + args.sample_folder)

    logging.info("Starting to create rebinned and remapped histograms")
    for path, binning in hist_info:
        rebin_bdt_output(samples, path, binning)

    if args.output_file:
        new_file_name = args.output_file
    else:
        new_file_name = args.input_file.split(".root")[0] + "_rebin.root"
    if samples.writeToFile(new_file_name, True, 2, False):
        logging.info("Writing modified sample folder to %s was successful", new_file_name)


if __name__ == "__main__":
    # setup parser
    PARSER = argparse.ArgumentParser("Apply the BDT rebinning")
    PARSER.add_argument("input_file", metavar="INPUT_FILE", type=str, help="Path of input file")
    PARSER.add_argument(
        "--sample-folder", dest="sample_folder", type=str, help="Name of sample folder", default="samples"
    )
    PARSER.add_argument("--output-file", dest="output_file", type=str, help="Path of output file", default="")
    PARSER.add_argument("--debug", dest="debug", action="store_true", help="Print debug messages", default=False)

    # setup logger and log-level
    ARGS = PARSER.parse_args()
    LOG_LEVEL = logging.DEBUG if ARGS.debug else logging.INFO
    logging.basicConfig(
        level=LOG_LEVEL, format="%(asctime)s - %(levelname)s - %(message)s", datefmt="%Y-%m-%d %H:%M:%S"
    )

    # import QFramework stuff after parser, because it takes so long
    logging.info("Initializing...")
    from QFramework import TQSampleFolder, TQFolder, TQHistogramUtils

    # run the program
    main(ARGS)
