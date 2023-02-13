#!/bin/env python2
from ROOT import *
from QFramework import *

# we're not interested in the transient tree, because we'll exclusively use the TStore
TQSample.gUseTransientTree = False

# we don't want to get swamped by warning messages, so we set the error level to ignore everything that is not at least an error
gErrorIgnoreLevel = 2500

# setup a dummy sample folder structure
samples = TQSampleFolder("samples")
s = samples.getSample("test+")
s.setTreeLocation("/afs/cern.ch/work/d/dbuesche/public/xAODs/CAOD_HIGG5D2/CxAOD_00-12-00/WH125/user.dbuesche.mc15_13TeV.341100.Py8EG_A14NNPDF23LO_WlvH125_bb.HIGG5D2.12-0_CxAOD.root.35723268/user.dbuesche.6014585._000001.CxAOD.root:CollectionTree")

# setup a dummy cut
cut = TQCut("base","No cut","1.","1.")

# setup a visitor
vis = TQAnalysisSampleVisitor(cut,True)

# create a TStore and set it active
store = xAOD.TStore()
store.setActive()

# create a CxAODReader Algorithm 
cxaodreader = CxAODReaderAlgorithm(False,False)
cxaodreader.select(CxAODSelectors.IntAccessor("isVHLooseElectron"),
                   CxAODSelectors.IntAccessor("isVHLooseMuon"),
                   CxAODSelectors.IntAccessor("isVetoJet"),
                   CxAODSelectors.IntAccessor("passTauSelector")
);

# add it to the visitor
vis.addAlgorithm(cxaodreader)

# call the event loop
samples.visitMe(vis)

# print what the results look like
samples.printContents("rdt")



