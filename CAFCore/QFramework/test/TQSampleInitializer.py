#!/usr/bin/env python

import os
import unittest

from QFramework import TQSampleInitializer, TQTaggable, TQSample, \
    TQSampleFolder
from ROOT import TTree, TFile, TDirectory, TString

from QFramework import TQPythonTest

class TQSampleInitializerTest(TQPythonTest):
    """
    Test the TQUtils observable.
    """

    def setUp(self):
        # create temporary directory
        super(TQSampleInitializerTest, self).setUp()

        # create ROOT file and empty tree
        self.path = os.path.join(self.tempdir, "xxx.123456.ggg.root")
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

        self.f = None

        # create tags and sample
        self.tags = TQTaggable()
        self.sample = TQSample("SomeFancyBackground")
        self.sample.importTags(self.tags)

    def tearDown(self):
        """
        Close open files.
        """
        if self.f is not None:
            if self.f.IsOpen():
                self.f.Close()

    def test_constructor(self):
        """
        Check that an object can be created.
        """
        si = TQSampleInitializer(self.tempdir, 1)
        self.assertTrue(si is not None)

    def test_getTreeInformation_plain_wo_wildcard(self):
        si = TQSampleInitializer(self.tempdir, 1)
        si.setTagString("treeName", "this_is_a_tree")

        self.sample.setTagString("name", "123456")
        self.sample.setTagInteger("dsid", 123456)
        self.sample.setTagString("path", os.path.dirname(self.path))
        self.sample.setTagString(".xsp.filepath", self.path)

        sf = TQSampleFolder("base")
        sf.addSampleFolder(self.sample, "/bkg+")

        sf.visitSampleFolders(si, "/bkg")
        
        self.assertEqual(self.sample.getTagStringDefault(".init.filepath", ""),
            self.path)
        self.assertEqual(self.sample.getTagStringDefault(".init.treename", ""),
            "this_is_a_tree")

    def test_getTreeInformation_plain_w_wildcard(self):
        si = TQSampleInitializer(self.tempdir, 1)
        si.setTagString("treeName", "*another*")

        self.sample.setTagString("name", "123456")
        self.sample.setTagInteger("dsid", 123456)
        self.sample.setTagString("path", os.path.dirname(self.path))
        self.sample.setTagString(".xsp.filepath", self.path)

        sf = TQSampleFolder("base")
        sf.addSampleFolder(self.sample, "/bkg+")

        sf.visitSampleFolders(si, "/bkg")
        
        self.assertEqual(self.sample.getTagStringDefault(".init.filepath", ""),
            self.path)
        self.assertEqual(self.sample.getTagStringDefault(".init.treename", ""),
            "this_is_another_tree")

    def test_getTreeInformation_directories(self):
        si = TQSampleInitializer(self.tempdir, 1)
        si.setTagString("treeName", "this_is_a_dir/in_dir_tree")

        self.sample.setTagString("name", "123456")
        self.sample.setTagInteger("dsid", 123456)
        self.sample.setTagString("path", os.path.dirname(self.path))
        self.sample.setTagString(".xsp.filepath", self.path)

        sf = TQSampleFolder("base")
        sf.addSampleFolder(self.sample, "/bkg+")

        sf.visitSampleFolders(si, "/bkg")
        
        self.assertEqual(self.sample.getTagStringDefault(".init.filepath", ""),
            self.path)
        self.assertEqual(self.sample.getTagStringDefault(".init.treename", ""),
            "this_is_a_dir/in_dir_tree")

if __name__ == "__main__":
    unittest.main()
