#!/usr/bin/env python

from __future__ import division

import argparse
import array
import logging
import math
import os

from ROOT import gStyle, TCanvas, TH1F, TH2F, gPad, TGaxis, gROOT, TLine

from get_bdt_blinding import get_blinding_threshold


def get_optimal_binning(hist, min_count=10, default_width=0.1, bin_prec=3, smooth_bkg=False):
    """Calculate the optimal binning for the given histogram.

    The algorithm starts with high bins and merges bins if the total count of events is smaller
    than `min_count`. Then it merges the rest of the bins into bins with a width of `default_width`.

    Args:
        hist (TH1*): Histogram which should be rebinned.
        min_count (float): Minimum number of background events per bin.
        default_width (float): Default width of the bins (if not rebinned).
        bin_prec (int): Floating point precision for rounding bin edges.
        smooth_bkg (bool): If true, background has to be monotonically decreasing

    Returns:
        list(float): List of the new bin edges.
    """
    new_bins = [1]
    last_bin_edge = 1
    count = 0
    bin_numbers = list(range(1, hist.GetNbinsX() + 1))
    bin_numbers.reverse()
    first_merge = True
    last_count = 0
    for i in bin_numbers:
        count += hist.GetBinContent(i)
        bin_low_edge = round(hist.GetBinLowEdge(i), bin_prec)
        if count >= min_count and (not smooth_bkg or count >= last_count):
            current_bin_width = round(abs(last_bin_edge - bin_low_edge), bin_prec)
            if smooth_bkg or (first_merge or current_bin_width >= default_width):
                new_bins.append(bin_low_edge)
                last_bin_edge = bin_low_edge
                last_count = count
                count = 0
                first_merge = False
    if new_bins[-1] != -1:  # last bin has to few entries
        new_bins[-1] = -1  # merge with previous bin
    new_bins.reverse()
    return new_bins


class BinUncertainties(object):
    """Manage the uncertainties for several bins which will be eventually merged.

    Attributes:
        counts (list(float)): A list of the contents in each bin.
        abs_uncertainties (list(float)): A list of the absolute uncertainties in each bin.
    """

    def __init__(self):
        self.counts = list()
        self.abs_uncertainties = list()

    def add_bin(self, bin_content, bin_error):
        """Add a bin.

        Arguments:
            bin_content (float): content of the bin
            bin_error (float): uncertainty of the bin
        """
        self.counts.append(bin_content)
        self.abs_uncertainties.append(bin_error)

    def get_relative_uncertainty(self):
        """Calculate the absolute relative uncertainty if all bins are merged.

        Returns:
            float: The absolute relative uncertainty if the bins are merged. 1000.0 if there are no events.
        """
        total_bkg = abs(sum(self.counts))
        total_uncert = math.sqrt(sum([uncert ** 2 for uncert in self.abs_uncertainties]))
        if total_bkg > 0:
            return total_uncert / total_bkg
        return 1000.0

    def get_count(self):
        """Calcualte the total amount of events.

        Returns:
            float: The total amount of events..
        """
        return sum(self.counts)


class NoMergeReason(object):
    """Enum for different reasons why the current bin selection was not merged."""

    bkg_count = 0
    rel_uncert = 1
    sbr_or_bkg_count_50 = 2
    rel_uncert_ttbar = 3
    rel_uncert_stop = 4
    rel_uncert_zll = 5
    rel_uncert_other = 6
    sbr = 7
    bkg_count_50 = 8


def get_run1_like_binning(reader, hist_string):
    """Calculate the binning like in Run-1.

    Bins are merged from right to left (high to low BDT score),
    until the following criteria are met:

    - The total expected background in the new bin exceeds the total expected
      background in the previous bin.

    - The relative statistical uncertainty of each background component is smaller
      than the defined threshold. This is currently 100% for every background.
      The backgrounds are combined to the following backgrounds:
        - ttbar
        - stop
        - Zll
        - other (diboson, ztautau, fakes, higgs)

    - Either the signal-to-background ratio is at most 10% smaller than in the
      previous bin or the total expected background exceeds the one in the previous
      bin by more than 50%.

    Returns:
        list(float): A list with the new binning (bin borders).
    """
    bin_prec = 5  # round bins to this precision

    new_merge_bins = []
    new_bin_edges = [1]

    hist_name = hist_string.split("/")[1]
    thresholds = dict()
    thresholds["top/ttbar"] = 0.3
    thresholds["top/stop"] = 0.5
    thresholds["Zee+Zmm"] = 0.7
    thresholds["diboson+Ztt+fakes+Higgs"] = 0.7

    total_background_name = "top+Zee+Zmm+diboson+Ztt+fakes+Higgs"
    total_signal_name = "nonres"

    bkg_hists = dict()
    for bkg in thresholds.keys():
        bkg_hists[bkg] = reader.getHistogram("/bkg/?/[" + bkg + "]", hist_string)
    total_bkg_hist = reader.getHistogram("/bkg/?/[" + total_background_name + "]", hist_string)
    total_sig_hist = reader.getHistogram("/sig/?/[" + total_signal_name + "]", hist_string)

    bin_numbers = list(range(1, total_bkg_hist.GetNbinsX() + 1))
    bin_numbers.reverse()
    first_merge = True
    current_total_background_count = 0
    last_total_background_count = 0
    current_uncertainties = dict([(bkg, BinUncertainties()) for bkg in thresholds])
    current_total_signal_count = 0
    current_sbr = 0
    last_sbr = 0

    hist_nomerge_reason = TH1F("nomerge_reason_{}".format(hist_name), "", 9, 0, 9)
    hist_nomerge_reason.SetDirectory(0)
    hist_nomerge_reason.GetXaxis().SetBinLabel(NoMergeReason.bkg_count + 1, "bkg_count")
    hist_nomerge_reason.GetXaxis().SetBinLabel(NoMergeReason.rel_uncert + 1, "rel_uncert")
    hist_nomerge_reason.GetXaxis().SetBinLabel(NoMergeReason.sbr_or_bkg_count_50 + 1, "sbr || bkg_count_50")
    hist_nomerge_reason.GetXaxis().SetBinLabel(NoMergeReason.rel_uncert + 1, "rel_uncert")
    hist_nomerge_reason.GetXaxis().SetBinLabel(NoMergeReason.rel_uncert_ttbar + 1, "rel_uncert_ttbar")
    hist_nomerge_reason.GetXaxis().SetBinLabel(NoMergeReason.rel_uncert_stop + 1, "rel_uncert_stop")
    hist_nomerge_reason.GetXaxis().SetBinLabel(NoMergeReason.rel_uncert_zll + 1, "rel_uncert_zll")
    hist_nomerge_reason.GetXaxis().SetBinLabel(NoMergeReason.rel_uncert_other + 1, "rel_uncert_other")
    hist_nomerge_reason.GetXaxis().SetBinLabel(NoMergeReason.sbr + 1, "sbr")
    hist_nomerge_reason.GetXaxis().SetBinLabel(NoMergeReason.bkg_count_50 + 1, "bkg_count_50")
    hist_nomerge_reason.GetXaxis().SetTitle("no-merge reason")
    hist_nomerge_reason.GetYaxis().SetTitle("number of occurrences")

    hist_nomerge_reason_2d = TH2F(
        "nomerge_reason_{}".format(hist_name),
        "",
        9,
        0,
        9,
        total_bkg_hist.GetNbinsX(),
        1,
        total_bkg_hist.GetNbinsX() + 1,
    )
    hist_nomerge_reason_2d.SetDirectory(0)
    hist_nomerge_reason_2d.GetXaxis().SetBinLabel(NoMergeReason.bkg_count + 1, "bkg_count")
    hist_nomerge_reason_2d.GetXaxis().SetBinLabel(NoMergeReason.rel_uncert + 1, "rel_uncert")
    hist_nomerge_reason_2d.GetXaxis().SetBinLabel(NoMergeReason.sbr_or_bkg_count_50 + 1, "sbr || bkg_count_50")
    hist_nomerge_reason_2d.GetXaxis().SetBinLabel(NoMergeReason.rel_uncert_ttbar + 1, "rel_uncert_ttbar")
    hist_nomerge_reason_2d.GetXaxis().SetBinLabel(NoMergeReason.rel_uncert_stop + 1, "rel_uncert_stop")
    hist_nomerge_reason_2d.GetXaxis().SetBinLabel(NoMergeReason.rel_uncert_zll + 1, "rel_uncert_zll")
    hist_nomerge_reason_2d.GetXaxis().SetBinLabel(NoMergeReason.rel_uncert_other + 1, "rel_uncert_other")
    hist_nomerge_reason_2d.GetXaxis().SetBinLabel(NoMergeReason.sbr + 1, "sbr")
    hist_nomerge_reason_2d.GetXaxis().SetBinLabel(NoMergeReason.bkg_count_50 + 1, "bkg_count_50")
    hist_nomerge_reason_2d.GetXaxis().SetTitle("no-merge reason")
    hist_nomerge_reason_2d.GetXaxis().SetTitleOffset(1.3)
    hist_nomerge_reason_2d.GetYaxis().SetTitle("bin")
    hist_nomerge_reason_2d.GetYaxis().SetTitleOffset(1.3)

    for i in bin_numbers:
        logging.debug("Bin number %d", i)
        # The total expected background in the new bin exceeds the total expected background in the previous bin
        current_total_background_count += total_bkg_hist.GetBinContent(i)
        if first_merge:
            cond_bkg_count = True
        else:
            cond_bkg_count = current_total_background_count > last_total_background_count
        if not cond_bkg_count:
            hist_nomerge_reason.Fill(NoMergeReason.bkg_count)
            hist_nomerge_reason_2d.Fill(NoMergeReason.bkg_count, i)

        # The relative statistical uncertainty of each background component is smaller
        # than the defined threshold
        # The yield for each group of backgrounds has to be bigger than 0
        cond_rel_uncert = True
        for bkg, bin_uncert in current_uncertainties.items():
            hist = bkg_hists[bkg]
            bin_uncert.add_bin(hist.GetBinContent(i), hist.GetBinError(i))
            rel_uncert = bin_uncert.get_relative_uncertainty()
            count = bin_uncert.get_count()
            # check if rel_uncert is not none and smaller than threshold
            logging.debug("%s = %f", bkg, count)
            cond_rel_uncert_single = rel_uncert and rel_uncert < thresholds[bkg]
            cond_rel_uncert = cond_rel_uncert and cond_rel_uncert_single
            if not cond_rel_uncert_single:
                if "ttbar" in bkg:
                    hist_nomerge_reason.Fill(NoMergeReason.rel_uncert_ttbar)
                    hist_nomerge_reason_2d.Fill(NoMergeReason.rel_uncert_ttbar, i)
                elif "stop" in bkg:
                    hist_nomerge_reason.Fill(NoMergeReason.rel_uncert_stop)
                    hist_nomerge_reason_2d.Fill(NoMergeReason.rel_uncert_stop, i)
                elif "Zee" in bkg:
                    hist_nomerge_reason.Fill(NoMergeReason.rel_uncert_zll)
                    hist_nomerge_reason_2d.Fill(NoMergeReason.rel_uncert_zll, i)
                else:
                    hist_nomerge_reason.Fill(NoMergeReason.rel_uncert_other)
                    hist_nomerge_reason_2d.Fill(NoMergeReason.rel_uncert_other, i)
        if not cond_rel_uncert:
            hist_nomerge_reason.Fill(NoMergeReason.rel_uncert)
            hist_nomerge_reason_2d.Fill(NoMergeReason.rel_uncert, i)

        # the signal-to-background ratio is at most 10% smaller than in the previous bin
        current_total_signal_count += total_sig_hist.GetBinContent(i)
        cond_sbr = True
        if first_merge:
            cond_sbr = True
        else:
            if current_total_signal_count > 0:
                current_sbr = current_total_signal_count / current_total_background_count
            else:
                current_sbr = -999
            cond_sbr = current_sbr > 0.9 * last_sbr
        if not cond_sbr:
            hist_nomerge_reason.Fill(NoMergeReason.sbr)
            hist_nomerge_reason_2d.Fill(NoMergeReason.sbr, i)

        # the total expected background exceeds the one in the previous bin by more than 50%.
        if first_merge:
            cond_bkg_count_50 = True
        else:
            cond_bkg_count_50 = current_total_background_count > 1.5 * last_total_background_count
        if not cond_bkg_count_50:
            hist_nomerge_reason.Fill(NoMergeReason.bkg_count_50)
            hist_nomerge_reason_2d.Fill(NoMergeReason.bkg_count_50, i)

        if not (cond_sbr or cond_bkg_count_50):
            hist_nomerge_reason.Fill(NoMergeReason.sbr_or_bkg_count_50)
            hist_nomerge_reason_2d.Fill(NoMergeReason.sbr_or_bkg_count_50, i)

        if cond_bkg_count and cond_rel_uncert and (cond_sbr or cond_bkg_count_50):
            bin_low_edge = round(total_bkg_hist.GetBinLowEdge(i), bin_prec)
            logging.debug("Merging at %g", bin_low_edge)
            new_merge_bins.append(i)
            new_bin_edges.append(bin_low_edge)

            logging.debug("Relative background uncertainties")
            for bkg, bin_uncert in current_uncertainties.items():
                logging.debug("    {}: {}".format(bkg, bin_uncert.get_relative_uncertainty()))

            first_merge = False
            last_total_background_count = current_total_background_count
            current_total_background_count = 0
            current_uncertainties = dict([(bkg, BinUncertainties()) for bkg in thresholds])
            current_total_signal_count = 0
            last_sbr = current_sbr

    if not os.path.exists("results/bdt_binning"):
        os.makedirs("results/bdt_binning")

    canvas = TCanvas("canvas", "", 800, 600)
    hist_nomerge_reason.Draw()
    canvas.SaveAs("results/bdt_binning/{}.pdf".format(hist_name))

    gStyle.SetPalette(97)
    canvas2 = TCanvas("canvas2", "", 800, 1200)
    hist_nomerge_reason_2d.Draw("COL")
    canvas2.Update()

    # separate top-level and individual rel. uncertainty reasons
    line_1 = TLine(3, 1, 3, len(bin_numbers) + 1)
    line_1.SetLineWidth(2)
    line_1.Draw()
    # separate individual rel. uncertainty and individual 3rd top-level reasons
    line_2 = TLine(7, 1, 7, len(bin_numbers) + 1)
    line_2.SetLineWidth(2)
    line_2.Draw()

    merge_lines = list()
    for merge_bin in new_merge_bins:
        merge_line = TLine(0, merge_bin, 9, merge_bin)
        merge_line.SetLineWidth(2)
        merge_line.Draw()
        merge_lines.append(merge_line)

    bdt_score_axis = TGaxis(gPad.GetUxmax(), gPad.GetUymin(), gPad.GetUxmax(), gPad.GetUymax(), -1, 1, 510, "+L")
    bdt_score_axis.SetTitle("BDT score")
    bdt_score_axis.SetTitleOffset(1.2)
    bdt_score_axis.SetLabelFont(hist_nomerge_reason_2d.GetXaxis().GetLabelFont())
    bdt_score_axis.SetTitleFont(hist_nomerge_reason_2d.GetXaxis().GetTitleFont())
    bdt_score_axis.Draw()
    canvas2.SaveAs("results/bdt_binning/{}_2d.pdf".format(hist_name))

    if len(new_bin_edges) == 1:  # no rebinning was done
        new_bin_edges.append(-1)
    if new_bin_edges[-1] != -1:  # last bin has to few entries
        new_bin_edges.append(-1)  # add last bin edge
    new_bin_edges.reverse()
    return new_bin_edges


def main(args):
    """Entry point of this script."""
    gROOT.ProcessLine("gErrorIgnoreLevel = 1001;")  # disalbe INFO messages
    gROOT.SetBatch(True)  # enable batch mode
    gStyle.SetOptStat(0)  # disable statistics box

    samples = TQSampleFolder.loadLazySampleFolder(args.input_file + ":" + args.sample_folder)
    reader = TQSampleDataReader(samples)

    options = ".includeUnderflow:True .includeOverflow:True"

    # this list contains 2-tuples with ("CutName", "HistogramName")
    hist_info = list()
    hist_info.append(("CutMll", "BDT_NONRES_SF_FINE"))
    hist_info.append(("CutMll", "BDT_NONRES_DF_FINE"))

    logging.info("Getting optimal binning")
    if args.run_1_like:
        logging.info("    Using Run-1-like binning algorithm")
    else:
        logging.info("    Minimum number of background events per bin: %g", args.min_count)
        logging.info("    Default bin width: %g", args.default_bin_width)
        logging.info("    Smooth (monotonically decreasing) background: %s", str(args.smooth_bkg))

    for cut_name, histogram_name in hist_info:
        logging.debug("Processing histogram %s/%s", cut_name, histogram_name)
        hist_sig = reader.getHistogram("sig/", "{}/{}".format(cut_name, histogram_name))
        hist_bkg = reader.getHistogram("bkg/", "{}/{}".format(cut_name, histogram_name))
        if not args.run_1_like:
            binning = get_optimal_binning(hist_bkg, args.min_count, args.default_bin_width, smooth_bkg=args.smooth_bkg)
        else:
            binning = get_run1_like_binning(reader, "{}/{}".format(cut_name, histogram_name))
        hist_sig_rebin = hist_sig.Rebin(
            len(binning) - 1, "{}/{}_rebin".format(cut_name, histogram_name), array.array("d", binning)
        )
        blinding = get_blinding_threshold(hist_sig_rebin, args.threshold)

        logging.info("{}/{}".format(cut_name, histogram_name))
        logging.info("    Binning: {}".format(str(binning)))
        logging.info("    Number of bins: {}".format(len(binning)))
        logging.info("    Blinding: >= {}".format(blinding))


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser("Get the optimized binning for the BDT distributions")
    PARSER.add_argument("input_file", metavar="INPUT_FILE", type=str, help="Input file")
    PARSER.add_argument(
        "--sample-folder", dest="sample_folder", type=str, help="Name of sample folder", default="samples"
    )
    PARSER.add_argument(
        "--threshold", type=float, help="Unblind up to this fraction of the total signal yield", default=0.3
    )
    PARSER.add_argument(
        "--min-count", dest="min_count", type=float, help="Minimum number of background events in each bin", default=10
    )
    PARSER.add_argument(
        "--default-bin-width", dest="default_bin_width", type=float, help="Default bin width for rebinning", default=0.1
    )
    PARSER.add_argument(
        "--run-1-like",
        dest="run_1_like",
        action="store_true",
        help="Use Run-1 (lephad) binning algorithm",
        default=True,
    )
    PARSER.add_argument(
        "--simple", dest="run_1_like", action="store_false", help="Use simple binning algorithm", default=True
    )
    PARSER.add_argument(
        "--smooth-bkg",
        dest="smooth_bkg",
        action="store_true",
        help="For simple binning algorithm: background has to be monotonically decreasing",
        default=False,
    )
    PARSER.add_argument("--debug", dest="debug", action="store_true", help="Print debug messages", default=False)
    ARGS = PARSER.parse_args()
    LOG_LEVEL = logging.DEBUG if ARGS.debug else logging.INFO
    logging.basicConfig(
        level=LOG_LEVEL, format="%(asctime)s - %(levelname)s - %(message)s", datefmt="%Y-%m-%d %H:%M:%S"
    )

    from QFramework import TQSampleFolder, TQSampleDataReader

    main(ARGS)
