#!/usr/bin/env python

import unittest

import ROOT
from QFramework import uhepp
from QFramework import TQPythonTest

class UheppPlotterTestCase(TQPythonTest):
    """Test the implementation of the QFramework.uhepp plotter"""

    def test_resolve_color_int(self):
        """Check that resolve_color resolves integer strings"""
        rgb = uhepp.resolve_color("2")
        self.assertEqual(rgb, "#ff0000")

        rgb = uhepp.resolve_color("5")
        self.assertEqual(rgb, "#ffff00")

    def test_resolve_color_const(self):
        """Check that resolve_color resolves kX constants"""
        rgb = uhepp.resolve_color("kRed")
        self.assertEqual(rgb, "#ff0000")

        rgb = uhepp.resolve_color("kBlue")
        self.assertEqual(rgb, "#0000ff")


    def test_resolve_expr(self):
        """Check that resolve_color resolves simple expressions"""
        rgb = uhepp.resolve_color("kRed + 1")
        self.assertEqual(rgb, "#cc0000")

        rgb = uhepp.resolve_color("kBlue-1")
        self.assertEqual(rgb, "#333366")

    def test_tex_plain_string(self):
        """Check that a plain string remains unchanged"""
        tex = uhepp.tex("Hello ATLAS")
        self.assertEqual(tex, "Hello ATLAS")

    def test_tex_plain_dollarize(self):
        """Check that a string with tex is dollarized"""
        tex = uhepp.tex("Z 1\\pm 2")
        self.assertEqual(tex, "$Z 1\\pm 2$")

    def test_tex_plain_dollarize_2(self):
        """Check that a string with tex is dollarized"""
        tex = uhepp.tex("Z_T^j")
        self.assertEqual(tex, "$Z_T^j$")

    def test_tex_plain_root_tex(self):
        """Check that ROOT-tex is converted to dollarized tex"""
        tex = uhepp.tex("Z #rightarrow #ell#ell")
        self.assertEqual(tex, "$Z \\rightarrow \\ell\\ell$")

    def test_tex_keep_tex(self):
        """Check that tex code is untouched"""
        tex = uhepp.tex("This is $1\\pm2$ \\LaTeX")
        self.assertEqual(tex, "This is $1\\pm2$ \\LaTeX")

    def test_tex_it(self):
        """Check that #it is removed"""
        tex = uhepp.tex("Hello #it{H\ell}")
        self.assertEqual(tex, "$Hello {H\ell}$")

    def test_th1_edges(self):
        """Check that the bin edges are extracted from a TH1"""
        th1 = ROOT.TH1F("", "", 4, 0, 1)
        edges = uhepp.get_th1_edges(th1)
        self.assertEqual(edges, [0, 0.25, 0.5, 0.75, 1])


if __name__ == "__main__":
    unittest.main()
