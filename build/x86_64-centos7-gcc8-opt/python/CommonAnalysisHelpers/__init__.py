# this is just the __init__ file

# provide defaults
from argparse import Namespace
defaultConfig = Namespace(
    sampleFolderName="samples",
    energyUnit="TeV",
    XsecUnit="pb",
    luminosityUnit="pb",
    writeSplitLevel=2,
    pathToConfig="info/config",
    pathToXSP="info/xsp",
    XSPName="TQXSecParser",
    pathToMCDirStruct="info/MCDirStruct",
    pathToCuts="info/cuts",
    pathToNormalization="info/normalization"
)

# sanitize argument handling
try:
    import ROOT
    ROOT.PyConfig.IgnoreCommandLineOptions = True

    import sys
    import QFramework
    if sys.argv[0]:
        from os.path import basename,splitext
        QFramework.TQLibrary.getQLibrary().setApplicationName(splitext(basename(sys.argv[0]))[0])
    else:
        QFramework.TQLibrary.getQLibrary().setApplicationName('pyROOTsession')
except:
    pass

# bugfix the 'format' method of c++-type strings
def _format_string(self,*args,**kwargs):
    return str(self).format(*args,**kwargs)
ROOT.string.__format__ = _format_string
ROOT.TString.__format__ = _format_string
