import QFramework
import ROOT

# attempt to load RooFitUtils
if QFramework.TQLibrary.hasPackage("RooFitUtils"):
    ROOT.gSystem.Load("libRooFitUtils.so")
