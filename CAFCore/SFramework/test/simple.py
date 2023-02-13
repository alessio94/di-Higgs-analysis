#!/usr/bin/env python

import os
import sys
import tempfile
import shutil

from QFramework import TQPythonTest

usedir = None


class SimpleTest(TQPythonTest):

    def setUp(self):
        """
        Create temporary test directory.
        """
        import QFramework

        # create temporary directory
        if not usedir:
            self.test_dir = tempfile.mkdtemp()
            QFramework.TQPathManager.getPathManager().setWorkingDirectory(self.test_dir)
        else:
            QFramework.TQPathManager.getPathManager().setWorkingDirectory(usedir)
        
        super(SimpleTest, self).setUp()

        basepath = os.path.join(QFramework.TQLibrary.getTQPATH().Data(),"../SFramework/test")
        configpath = os.path.join(basepath,"simple.txt")
        self.config = QFramework.TQFolder.loadFromTextFile(configpath)
        self.assertIsInstance(self.config,QFramework.TQFolder)       

        samples = self.buildSampleFolder()        
        self.assertIsInstance(samples,QFramework.TQSampleFolder)

        import SFramework
        self.manager = SFramework.TSStatisticsManager()
        self.manager.setDefaultSampleFolder(samples)
        
    
    def tearDown(self):
        # delete temporary directory
        if not usedir:
            shutil.rmtree(self.test_dir)
        super(SimpleTest, self).tearDown()
        pass

    def buildSampleFolder(self):
        """
        create a simple input sample folder
        """

        from math import sqrt,pi
        import ROOT
        import QFramework
        
        hist = ROOT.TH1F("MyVar","MyVar",11,-1,1)
        hist.SetDirectory(0)
        tf = ROOT.TF1("uniform","1.")
        
        cnt = QFramework.TQCounter("CutCR")
        
        samples = QFramework.TQSampleFolder("samples")
        
        sig = samples.getSampleFolder("sig+")
        sighists = sig.getFolder(".histograms+")
        sigctflw = sig.getFolder(".cutflow+")        
        sigsr = sighists.getFolder("CutSR+")

        bkg = samples.getSampleFolder("bkg+")
        bkghists = bkg.getFolder(".histograms+")
        bkgctflw = bkg.getFolder(".cutflow+")        
        bkgsr = bkghists.getFolder("CutSR+")

        data = samples.getSampleFolder("data+")
        datahists = data.getFolder(".histograms+")
        datactflw = data.getFolder(".cutflow+")        
        datasr = datahists.getFolder("CutSR+")                
        
        sighist = QFramework.TQHistogramUtils.copyHistogram(hist)
        sighist.FillRandom("gaus")
        sighist.Scale(0.01)
        sigsr.addObject(sighist)
        sigcnt = QFramework.TQCounter(cnt)
        sigcnt.setCounter(1.0)
        sigcnt.setError(0.0001)
        sigctflw.addObject(sigcnt)
        
        bkghist = QFramework.TQHistogramUtils.copyHistogram(hist)
        bkghist.FillRandom("uniform")
        bkghist.Scale(0.1)
        bkgsr.addObject(bkghist)
        bkgcnt = QFramework.TQCounter(cnt)
        bkgcnt.setCounter(1000.)
        bkgcnt.setError(1.)
        bkgctflw.addObject(bkgcnt)        
        
        datahist = QFramework.TQHistogramUtils.copyHistogram(bkghist)
        datahist.Add(sighist)
        QFramework.TQHistogramUtils.rerollPoisson(datahist,5337)
        datasr.addObject(datahist)
        datacnt = QFramework.TQCounter(cnt)
        val = bkgcnt.getCounter() + datacnt.getCounter()
        datacnt.setCounter(val)
        datacnt.setError(sqrt(val))
        datactflw.addObject(datacnt)

        samples.writeToFile(QFramework.TQPathManager.getPathManager().getTargetPath("samples.root"))
        
        return samples

    def test_build_run_simple(self):
        """
        build a simple model and workspace, fit it
        """
        import ROOT
        ROOT.gROOT.SetBatch(True)
        assert(self.manager.run(self.config))

        model = self.manager.getModels().getObject("Test")
        
        workspace = self.manager.getWorkspaces().getObject("Test")
        self.assertIsInstance(workspace,ROOT.RooWorkspace)
        
        import QFramework
        result = self.manager.getResults().getFolder("Test")
        self.assertIsInstance(result,QFramework.TQFolder)

        muAsimov = result.getFolder("Significance/asimov/FitResults/asimov_muhat/floatParsFinal/mu")
        z0Asimov = result.getFolder("Significance/asimov/").getTagDoubleDefault("Z0_asimov",0)
        self.assertIsInstance(result,QFramework.TQFolder)        
        muval = muAsimov.getTagDoubleDefault("val",0.)
        
        self.assertAlmostEqual(muval,1.,places=3)
        self.assertAlmostEqual(z0Asimov,1.7,places=1)   

        self.manager.reset()

    def test_modelops(self):
        """
        build a simple model, run some fancy operations
        """
        import ROOT
        from os.path import join as pjoin
        from random import expovariate as randexp
        from random import seed as randseed
        randseed(42)
        ROOT.gROOT.SetBatch(True)

        from QFramework import TQFolder
        config = TQFolder("config")
        modelconf = self.config.getFolder("CreateModels").copy()
        samples = []
        for sample in modelconf.getListOfFolders("?/Samples/?"):
            if sample.GetName() == "Data": continue
            samples.append(sample.GetName())
            sys = sample.getFolder("OverallSys.Lumi+")
            sys.setTagDouble("Val",1.)
            sys.setTagDouble("Low",.99)
            sys.setTagDouble("High",1.01)
        channels = []
        for channel in modelconf.getListOfFolders("?/Channels/?"):
            channels.append(channel.GetName())
        config.addFolder(modelconf)

        sysimportconf = config.getFolder("ImportSystematics/Test+")
        for channel in channels:
            for sample in samples:
                for i in range(0,10):
                    sysname = "Sys"+str(i)
                    path = pjoin(sysname,channel,sample)
                    sys = sysimportconf.getFolder(path+"+")
                    sys.setTagBool("IsOverallSys",True)
                    sys.setTagBool("IsHistoSys",False)
                    sys.setTagDouble("Val",1.)
                    sys.setTagDouble("Low",1.-randexp(100))
                    sys.setTagDouble("High",1.+randexp(100))
                    sys.setTagDouble("LowErr",randexp(20))
                    sys.setTagDouble("HighErr",randexp(20))

        sysprocessconf = config.getFolder("ProcessSystematics/Test+")
        sysprocessconf.setTagDouble("SysRelThreshold",0.005)
        sysprocessconf.setTagDouble("SysRelCutoff",0.8)
        sysprocessconf.setTagDouble("SysSigMin",0.005)
        sysprocessconf.setTagDouble("SysAsymLimit",0.)

        sysstatusconf = config.getFolder("PlotSystematicsStatus/Test+")
        sysstatusconf.setTagString("outputFile","systematics.html")
        sysstatusconf.setTagString("show","Percent")

        assert(self.manager.run(config))
        
        self.manager.reset()


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("--output",default=None,type=str)
    args = parser.parse_args()
    usedir = args.output

    sys.argv = [sys.argv[0]]
    import unittest
    unittest.main()
