# -*- coding: utf-8 -*-

"""Official ATLAS style ported to python"""

from ROOT import gROOT, gPad
from ROOT import TStyle, TLatex
from ROOT import kBlack

INT = "Internal"
WIP = "Work in progress"
SIM = "Simulation"
SIMWIP = "Simulation work in progress"


def build_atlas_style():
    """Build the ATLAS style"""
    atlas_style = TStyle("ATLAS", "Atlas style")

    # use plain black on white colors
    atlas_style.SetFrameBorderMode(0)
    atlas_style.SetFrameFillColor(0)
    atlas_style.SetCanvasBorderMode(0)
    atlas_style.SetCanvasColor(0)
    atlas_style.SetPadBorderMode(0)
    atlas_style.SetPadColor(0)
    atlas_style.SetStatColor(0)

    # set the paper & margin sizes
    atlas_style.SetPaperSize(20, 26)

    # set margin sizes
    atlas_style.SetPadTopMargin(0.05)
    atlas_style.SetPadRightMargin(0.05)
    atlas_style.SetPadBottomMargin(0.16)
    atlas_style.SetPadLeftMargin(0.16)

    # set title offsets (for axis label)
    atlas_style.SetTitleXOffset(1.4)
    atlas_style.SetTitleYOffset(1.4)

    # use large fonts
    font = 42  # Helvetica
    tsize = 0.05
    atlas_style.SetTextFont(font)

    atlas_style.SetTextSize(tsize)
    atlas_style.SetLabelFont(font, "x")
    atlas_style.SetTitleFont(font, "x")
    atlas_style.SetLabelFont(font, "y")
    atlas_style.SetTitleFont(font, "y")
    atlas_style.SetLabelFont(font, "z")
    atlas_style.SetTitleFont(font, "z")

    atlas_style.SetLabelSize(tsize, "x")
    atlas_style.SetTitleSize(tsize, "x")
    atlas_style.SetLabelSize(tsize, "y")
    atlas_style.SetTitleSize(tsize, "y")
    atlas_style.SetLabelSize(tsize, "z")
    atlas_style.SetTitleSize(tsize, "z")

    # use bold lines and markers
    atlas_style.SetMarkerStyle(20)
    atlas_style.SetMarkerSize(1.2)
    atlas_style.SetHistLineWidth(2)
    atlas_style.SetLineStyleString(2, "[12 12]")  # postscript dashes

    # get rid of X error bars
    # atlas_style.SetErrorX(0.001)
    atlas_style.SetErrorX(0.5)
    # get rid of error bar caps
    atlas_style.SetEndErrorSize(0.0)

    # do not display any of the standard histogram decorations
    atlas_style.SetOptTitle(0)
    atlas_style.SetOptStat(0)
    atlas_style.SetOptFit(0)

    # put tick marks on top and RHS of plots
    atlas_style.SetPadTickX(1)
    atlas_style.SetPadTickY(1)

    return atlas_style


def set_atlas_style():
    """Set the ATLAS style"""
    _ = build_atlas_style()
    gROOT.SetStyle("ATLAS")
    gROOT.ForceStyle()


def draw_atlas_label(x, y, text=""):
    """Draw the ATLAS label"""
    latex_atlas = TLatex()
    latex_atlas.SetNDC()
    latex_atlas.SetTextFont(72)
    latex_atlas.SetTextColor(kBlack)

    latex_atlas.DrawLatex(x, y, "ATLAS")
    if text:
        delta_x = 0.09 * 696 * gPad.GetWh() / (472 * gPad.GetWw())
        latex_text = TLatex()
        latex_text.SetNDC()
        latex_text.SetTextFont(42)
        latex_text.SetTextColor(kBlack)
        latex_text.DrawLatex(x + delta_x, y, text)
