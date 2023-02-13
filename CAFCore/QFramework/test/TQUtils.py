#!/usr/bin/env python

import os
import unittest

from QFramework import TQUtils
from ROOT import TTree, TFile, TClass, TDirectory

from QFramework import TQPythonTest

class TQUtilsTest(TQPythonTest):
    """
    Test the TQUtils observable.
    """

    def setUp(self):
        # create temporary directory
        super(TQUtilsTest, self).setUp()

        # create ROOT file and empty tree
        self.path = os.path.join(self.tempdir, "TQUtils.root")
        file = TFile(self.path, "recreate")
        tree = TTree("this_is_a_tree", "")
        tree.Write()
        tree = TTree("this_is_another_tree", "")
        tree.Write()
        tree = TTree("no_tree", "")
        tree.Write()


        dir = file.mkdir("this_is_a_dir")
        dir.cd()
        tree = TTree("in_dir_tree", "")
        tree.SetDirectory(dir)
        tree.Write()

        file.Close()

        os.environ["TEST"] = "thisIsADummyTestValue"
        
        self.f = None

    def tearDown(self):
        """
        Close open files.
        """
        if self.f is not None:
            if self.f.IsOpen():
                self.f.Close()
        del os.environ["TEST"]

    def test_getObjects_plain_wo_wildcard(self):
        """
        Check that the three trees can be retrieved, when the exact names are
        given to getObjects.
        """
        self.f = TFile(self.path)
        cls = TClass.GetClass("TTree")

        l = TQUtils.getObjects("this_is_a_tree", cls, self.f)
        self.assertEqual(len(l), 1)
        self.assertEqual(l[0].GetName(), "this_is_a_tree")

        l = TQUtils.getObjects("this_is_another_tree", cls, self.f)
        self.assertEqual(len(l), 1)
        self.assertEqual(l[0].GetName(), "this_is_another_tree")

        l = TQUtils.getObjects("no_tree", cls, self.f)
        self.assertEqual(len(l), 1)
        self.assertEqual(l[0].GetName(), "no_tree")


    def test_getObjects_plain_w_wildcard(self):
        """
        Check that the three trees can be retrieved, when the query names use
        wildcards.
        """
        self.f = TFile(self.path)
        cls = TClass.GetClass("TTree")

        l = TQUtils.getObjects("this_is_a_*", cls, self.f)
        self.assertEqual(len(l), 1)
        self.assertEqual(l[0].GetName(), "this_is_a_tree")

        l = TQUtils.getObjects("this_is_another*", cls, self.f)
        self.assertEqual(len(l), 1)
        self.assertEqual(l[0].GetName(), "this_is_another_tree")

        l = TQUtils.getObjects("no_*", cls, self.f)
        self.assertEqual(len(l), 1)
        self.assertEqual(l[0].GetName(), "no_tree")

        l = TQUtils.getObjects("*_tree", cls, self.f)
        self.assertListEqual(sorted([x.GetName() for x in l]),
            ["no_tree", "this_is_a_tree", "this_is_another_tree"])

        l = TQUtils.getObjects("*_another*", cls, self.f)
        self.assertEqual(len(l), 1)


    def test_getObjects_subdirs(self):
        """
        Check that the three trees can be retrieved, when the query is a exact
        path with subdirectories.
        """
        self.f = TFile(self.path)
        cls = TClass.GetClass("TTree")

        l = TQUtils.getObjects("this_is_a_dir/in_dir_tree", cls, self.f)
        self.assertEqual(len(l), 1)
        self.assertEqual(l[0].GetName(), "in_dir_tree")
    
    def test_readEnvVar(self):
        
        out = TQUtils.readEnvVarValue("TEST")
        self.assertEqual(out, os.environ["TEST"])


if __name__ == "__main__":
    unittest.main()
