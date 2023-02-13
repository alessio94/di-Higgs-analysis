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
        # create temporary directory
        if not usedir:
            self.test_dir = tempfile.mkdtemp()
            os.chdir(self.test_dir)
        else:
            os.chdir(usedir)   

        super(SimpleTest, self).setUp()

        import SFramework
        self.manager = SFramework.TSStatisticsManager()
        self.manager.getWorkspaces().addObject(self.makeWS())
        
    def tearDown(self):
        # delete temporary directory
        if not usedir:
            shutil.rmtree(self.test_dir)   
        super(SimpleTest, self).tearDown()

    def makeWS(self):
        """
        Create the workspace to run on
        """
        # adopted from https://root.cern.ch/root/html/tutorials/roofit/rf601_intminuit.C.html
        import ROOT

        # observable
        x = ROOT.RooRealVar("x","x",-20,20) 

        # Model (intentional strong correlations)
        mean = ROOT.RooRealVar("mean","mean of g1 and g2",0)
        
        sigma_g1 = ROOT.RooRealVar("sigma_g1","width of g1",3,1,20) 
        g1 = ROOT.RooGaussian("g1","g1",x,mean,sigma_g1) 

        sigma_g2 = ROOT.RooRealVar("sigma_g2","width of g2",4,3,6) 
        g2 = ROOT.RooGaussian("g2","g2",x,mean,sigma_g2) 

        frac = ROOT.RooRealVar("frac","frac",0.5,0.0,1.0) 
        model = ROOT.RooAddPdf("model","model",g1,g2,frac) 

        # Generate 1000 events
        args = ROOT.RooArgSet(x)
        data = model.generate(args,1000) ;

        ws = ROOT.RooWorkspace("2d")
        
        import SFramework
        SFramework.TSUtils.importObjectToWorkspace(ws,model)
        SFramework.TSUtils.importObjectToWorkspace(ws,data)

        return ws

    def test_contour(self):

        import QFramework
        basepath = os.path.join(QFramework.TQLibrary.getTQPATH().Data(),"../SFramework/test")
        configpath = os.path.join(basepath,"contour.txt")
        config = QFramework.TQFolder.loadFromTextFile(configpath)
        
        self.manager.run(config)
#        self.manager.getResults().printContents("r3dt")
        
        pass        
        
if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("--output",default=None,type=str)
    args = parser.parse_args()
    usedir = args.output

    sys.argv = [sys.argv[0]]
    import unittest
    unittest.main()
