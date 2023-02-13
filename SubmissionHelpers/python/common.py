#!/usr/bin/env python

import sys

# custom implementation of six.string_types as six package is not installed everywhere (e.g. containers, etc)
# https://github.com/benjaminp/six/blob/master/six.py
if sys.version_info[0] == 2:
    string_types = basestring
else:
    string_types = str

def mkdir_p(path):
  """create a directory, emulating the behavior of 'mkdir -p'"""
  import os
  import errno
  try:
    os.makedirs(path)
  except OSError as exc:
    if exc.errno == errno.EEXIST and os.path.isdir(path):
      pass
    else: raise

def mkdir_forFile(filepath):
  from os.path import dirname,isdir
  if not isdir(dirname(filepath)):
    mkdir_p(dirname(filepath))

def concat(pieces,sep=" "):
  if isstr(pieces):
    return pieces
  else:
    return sep.join(pieces)

def mergeDicts(dictA, dictB):
    #merges two dicts of lists by appending entries of lists from dictB to the lists in dictA where the lists have the same keys in both dicts. Note: dictA will be modified
    if not (isinstance(dictA,dict) and isinstance(dictB,dict)):
      raise TypeError("Error in submissionControllerBase.__mergeDicts__ : at least one the the arguments provided is not a dict!")
      return
    for key in ( set(dictA.keys()) + set(dictB.keys()) ):
      if key in dictA and key in dictB:
        #case 1: key is in both dicts, so append B to A
        if not isinstance(dictA[key],list):
          raise TypeError("Error in mergeDicts: key '{:s}' in dictA does not resolve to a list!".format(key))
          return False
        if not isinstance(dictB[key],list):
          raise TypeError("Error in mergeDicts: key '{:s}' in dictB does not resolve to a list!".format(key))
          return False
        dictA[key].extend(dictB[key])
      elif key in dictB:
        #case 2: key is only in dictB, so just copy it to dictA unless it's not a list
        if not isinstance(dictB[key],list):
          raise TypeError("Error in submissionControllerBase.__mergeDicts__ : key '{:s}' in dictB does not resolve to a list!".format(key))
          return False
        dictA[key] = dictB[key]
      #case 3: key is only in dictA but not in dictB: nothing to do.
    return True

def isstr(s):
    return isinstance(s, string_types)

def createEmptyArgumentParser(description):
    """Create an empty argument parser"""
    from argparse import ArgumentParser
    parser = ArgumentParser(description=description, conflict_handler='resolve') # conflict_handler: allow to overwrite existing options with the same name
    return parser

def locateLibraries():
    try:
        # make root optional
        import ROOT
    except ImportError:
        pass
    from sys import modules
    libs = {}
    for m in modules.values():
      try:
        if m.__file__ and ".so" in m.__file__:
          libs[m.__name__] = m.__file__
      except AttributeError:
        pass
    return libs

def findSetupScript(lib,mindepth=0):
    from os.path import split as psplit
    from os.path import join as pjoin
    from glob import glob
    path,filename = psplit(lib)
    if not path: return None
    if path.count("/") < mindepth: return None
    p = pjoin(path,"*setup*.sh")
    setup = glob(p)
    if setup: return setup
    return findSetupScript(path,mindepth+1)

def findLibrarySetupScripts():
    libs = locateLibraries()
    elements = set()
    for lib in libs.values():
        scripts = findSetupScript(lib,2)
        if not scripts: continue
        for s in scripts:
              elements.add(s)
    return elements

def printLibrarySetupScripts():
  for e in findLibrarySetupScripts():
    print(e)
