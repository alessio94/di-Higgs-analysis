def loadlib(libname,errorLevel=1):
  import ROOT
  loaded = ROOT.gInterpreter.GetSharedLibs().split()
  pathname = ROOT.gSystem.DynamicPathName(libname,True)
  if pathname in loaded:
    return
  if not pathname:
    if errorLevel == 1:
      print("unable to load library "+libname)
    elif errorLevel == 2:
      raise ImportError("\033[1;31mFATAL\033[0m: unable to find library '{:s}'".format(libname))
  else:
    try:
      error = ROOT.gSystem.Load(libname)
    except Exception as ex:
      template = "unable to load '"+libname+"' - an exception of type '{0}' occured: {1!s}"
      message = template.format(type(ex).__name__, ",".join(ex.args))
      raise ImportError("\033[1;31mFATAL\033[0m: "+message)

# import basics
from os import getenv
from os.path import join as pjoin
from os.path import dirname,isfile
from distutils.version import StrictVersion
from platform import python_version
import re

import ROOT
# check if the python version is compatible
pyVersion = python_version()
if pyVersion.find("rc") >0:
  pyVersion = pyVersion[:pyVersion.find("rc")]
if StrictVersion(pyVersion) < StrictVersion('2.7.0'):
  raise ImportError("\033[1;31mFATAL\033[0m: unsupported python version, please use at least version 2.7.0")

# we will need this library later
loadlib("libTreePlayer")

#@env: [CAF_LIBRARIES] Paths specified in this env var are added to the 'dynamic path' when the QFramework python module is loaded
moreLibs = getenv("CAF_LIBRARIES")
if moreLibs:
  ROOT.gSystem.AddDynamicPath(moreLibs)

#@env: [CAF_PRELOAD_LIBRARIES] this env var specifies a list of library names which should be automatically loaded when the QFramework python module is loaded.
PRELOAD_LIBS=getenv("CAF_PRELOAD_LIBRARIES","")
for preLoadLib in re.split('[,;: ]+',PRELOAD_LIBS):
  if len(preLoadLib)<1 : continue
  loadlib(preLoadLib)

# retrieve the root core dir environment variable
ROOTCOREBIN = getenv("ROOTCOREBIN")
CAF_BIN = getenv ("CAF_BIN")

if ROOTCOREBIN: # In an ASG environment
  if isfile(pjoin(dirname(ROOTCOREBIN),"rcSetup.sh")):
    # Compiling with RootCore
    ROOT.gROOT.ProcessLine(".x $ROOTCOREDIR/scripts/load_packages.C")
  else:
    # Compiling with CMake
    loadlib("libxAODRootAccess")
    ROOT.xAOD.Init().ignore()
  loadlib(ROOT.TQLibrary.getlibXMLpath().Data())
else: # In standalone environment
  # we will need these libraries later
  #@env: [LIBXMLPATH] path to the libxml2 library
  lxmlpath = getenv ("LIBXMLPATH")
  if lxmlpath:
    loadlib(lxmlpath,0)
  else:
    loadlib("lxml2",0)

  loadlib("libRooFit")
  loadlib("libQFramework",2)
  loadlib("libRooFitUtils",0)
  loadlib("libSFramework")

# define some print commands that are available as preprocessor macros in the library
def BREAK(arg):
  ROOT.TQLibrary.msgStream.sendMessage(ROOT.TQMessageStream.BREAK,arg)
    
def ERROR(arg):
  ROOT.TQLibrary.msgStream.sendMessage(ROOT.TQMessageStream.ERROR,arg)

def CRITICAL(arg):
  ROOT.TQLibrary.msgStream.sendMessage(ROOT.TQMessageStream.CRITICAL,arg)
  
def INFO(arg):
  ROOT.TQLibrary.msgStream.sendMessage(ROOT.TQMessageStream.INFO,arg)

def WARN(arg):
  ROOT.TQLibrary.msgStream.sendMessage(ROOT.TQMessageStream.WARNING,arg)

def START(align,msg):
  ROOT.TQLibrary.msgStream.startProcessInfo(ROOT.TQMessageStream.INFO,min(ROOT.TQLibrary.getConsoleWidth(),120),align,msg)

def END(result):
  ROOT.TQLibrary.msgStream.endProcessInfo(result)

def parseException(ex):
  import re
  c = re.compile("[ \n]*(.*) =>\n[ ]*(.*)[ \n]*\(C\+\+ exception of type ([^ ]*)\)[ \n]*",re.DOTALL)
  m = c.search(ex.args[0])
  if m:
    return m.group(3),m.group(1),m.group(2).strip()
  else:
    raise RuntimeError("error parsing expression: '"+str(ex.args[0])+"'")
  
# provide sensible pretty-printing functionality for the basic classes
def TQTaggable__repr__(self):
  return "{:s}(\"{:s}\")".format(self.Class().GetName(),self.exportTagsAsString().Data())
ROOT.TQTaggable.__repr__ = TQTaggable__repr__

def TQTaggable__str__(self):
  return self.exportTagsAsString().Data()
ROOT.TQTaggable.__str__ = TQTaggable__str__

def TQFolder__repr__(self):
  return "{:s}(\"{:s}\") @ {:s}:{:s} {:s}".format(self.Class().GetName(),self.GetName(),self.getRoot().GetName(),self.getPath().Data(),self.exportTagsAsString().Data())
ROOT.TQFolder.__repr__ = TQFolder__repr__

def TQFolder__str__(self):
  return "{:s}:{:s} {:s}".format(ROOT.TQStringUtils.makeBoldBlue(self.getRoot().GetName()).Data(),ROOT.TQStringUtils.makeBoldWhite(self.getPath()).Data(),self.exportTagsAsString().Data())
ROOT.TQFolder.__str__ = TQFolder__str__


#some convenience tricks to make TString and python string more compatible
try:
  ROOT.TString.__format__
except:
  def TString__format__(self,format_spec):
    s = str(self.Data())
    return s.__format__(format_spec)
  ROOT.TString.__format__ = TString__format__

def TString__add__(self,other):
  if isinstance (other,ROOT.TString):
    return ROOT.TString(self.Data() + other.Data()) #simple case, both are TStrings
  else:
    return ROOT.TString(self.Data() + other) #the other object is not a TString, convert it (addition was called on a TString so return value should be TString)
def TString__radd__(self,other):
  return other + self.Data()
def TString__bool__(self):
  return not self.IsNull()
ROOT.TString.__add__ = TString__add__
ROOT.TString.__radd__ = TString__radd__
ROOT.TString.__bool__ = TString__bool__
ROOT.TString.__nonzero__ = TString__bool__


import unittest
class TQPythonTest(unittest.TestCase):
  """
  Base test for all QFramework test cases. This class provides a
  temporary directory which can be used to create external files. The
  directory is deleted when all test cases have been executed.
  """

  def setUp(self):
    """
    Create temporary directory.
    """
    import tempfile
    self.tempdir = tempfile.mkdtemp(prefix="qftest_")

  def tearDown(self):
    """
    Delete temporary directory.
    """   
    import os
    if os.path.exists(self.tempdir):
      import shutil
      shutil.rmtree(self.tempdir)

from .uheppplotter import Plotter as uhepp
