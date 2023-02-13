#!/usr/bin/env python

import argparse
import logging

from math import sqrt


def get_blinding_threshold_1(hist_sig, threshold, start_from_left=True):
    """Unblind all bins until the unblinded bins contain a given fraction of the total signal yield.

    Arguments:
        hist_sig (TH1*): Histogram with signal distribution.
        threshold (float): Unblind up to this fraction of the total signal yield.
        start_from_left (bool): If set to false, start from the right side of the distribution. (default=True)

    Returns:
        Bin edge until which can be unblinded.
    """
    total_sig = hist_sig.Integral()
    logging.debug("Total signal yield: %s", total_sig)
    current_sig = 0
    if start_from_left:
        bins = range(1, hist_sig.GetNbinsX() + 1)
    else:
        bins = range(hist_sig.GetNbinsX(), 0, -1)
    for i in bins:
        s = hist_sig.GetBinContent(i)
        logging.debug("Addig bin %d with value %f", i, s)
        current_sig += s
        logging.debug("Current signal yield is %f", current_sig)
        if current_sig >= threshold * total_sig:
            logging.debug("Exceeded threshold in bin %d", i)
            if start_from_left:
                # the upper edge of the previous bin is the lower edge of this bin
                return hist_sig.GetBinLowEdge(i)
            else:
                # return the high bin edge of this bin
                return hist_sig.GetBinLowEdge(i) + hist_sig.GetBinWidth(i)

    logging.error("No threshold found, this is not expected! Please investigate")
    logging.error("Current signal yield is %f, total signal yield is %f", current_sig, total_sig)


def get_blinding_threshold_2(hist_sig, hist_bkg, threshold, start_from_left=True):
    """Unblind all bins until the cummulative significance z=s/sqrt(b) is over a given threshold.

    Arguments:
        hist_sig (TH1*): Histogram with signal distribution.
        hist_sig (TH1*): Histogram with background distribution.
        start_from_left (bool): If set to false, start from the right side of the distribution.
        threshold (float): Unblind up to this cummulative significance.

    Returns:
        Bin edge until which can be unblinded.
    """
    sum_z = 0
    if start_from_left:
        bins = range(1, hist_sig.GetNbinsX() + 1)
    else:
        bins = range(hist_sig.GetNbinsX(), 0, -1)
    for i in bins:
        s = hist_sig.GetBinContent(i)
        b = hist_bkg.GetBinContent(i)
        z = s / sqrt(b)
        logging.debug("Bin %02d: s = %f, b = %f, z = %f", i, s, b, z)
        sum_z += z
        logging.debug("Current current cummulative significance z = %f", sum_z)
        if sum_z >= threshold:
            logging.debug("Exceeded threshold in bin %d", i)
            if start_from_left:
                # the upper edge of the previous bin is the lower edge of this bin
                return hist_sig.GetBinLowEdge(i)
            else:
                # return the high bin edge of this bin
                return hist_sig.GetBinLowEdge(i) + hist_sig.GetBinWidth(i)

    logging.info("No threshold found, cummulative significance z = %f", sum_z)
    return None


def main(args):
    samples = TQSampleFolder.loadLazySampleFolder(args.input_file + ":" + args.sample_folder)
    reader = TQSampleDataReader(samples)

    # this list contains 3-tuples with ("CutName", "HistogramName", start-from-left(bool))
    hist_info = list()
    hist_info.append(("Cut2TagMllSR1VBFVeto", "NN_SR1_Signal", True))
    hist_info.append(("Cut2TagMllSR1VBFVeto", "NN_SR1_Signal_Rebin", True))
    hist_info.append(("Cut2TagMllSR1VBFVeto", "NN_SR1_Top", False))
    hist_info.append(("Cut2TagMllSR1VBFVeto", "NN_SR1_Other", False))
    hist_info.append(("Cut2TagMbbSR2", "NN_SR2_Signal", True))
    hist_info.append(("Cut2TagMbbSR2", "NN_SR2_Signal_Rebin", True))
    hist_info.append(("Cut2TagMbbSR2", "NN_SR2_Top", False))
    hist_info.append(("Cut2TagMbbSR2", "NN_SR2_Zll", False))
    hist_info.append(("Cut2TagMbbSR2", "NN_SR2_Other", False))

    # get blinding thresholds
    for cut_name, histogram_name, start_from_left in hist_info:
        logging.debug("Processing histogram %s/%s", cut_name, histogram_name)
        hist_sig = reader.getHistogram(
            "sig/[ee+mm+em+me]/[c16a+c16d+c16e]/nonres/", "{}/{}".format(cut_name, histogram_name)
        )
        hist_bkg = reader.getHistogram("bkg/[ee+mm+em+me]/[c16a+c16d+c16e]/", "{}/{}".format(cut_name, histogram_name))
        blinding1 = get_blinding_threshold_1(hist_sig, args.threshold1, start_from_left)
        blinding2 = get_blinding_threshold_2(hist_sig, hist_bkg, args.threshold2, start_from_left)
        if start_from_left:
            sign = ">="
        else:
            sign = "<="
        print("Blinding (option 1) for {}/{}: {} {}".format(cut_name, histogram_name, sign, blinding1))
        print("Blinding (option 2) for {}/{}: {} {}".format(cut_name, histogram_name, sign, blinding2))


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser(description="Get the blinding threshold for the MVA distributions")
    PARSER.add_argument("input_file", metavar="INPUT_FILE", type=str, help="Input file")
    PARSER.add_argument(
        "--threshold1", type=float, help="Unblind up to this fraction of the total signal yield", default=0.15
    )
    PARSER.add_argument("--threshold2", type=float, help="Unblind up to cummulative s/sqrt(b) value", default=0.05)
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
