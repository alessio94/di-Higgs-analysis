#!/bin/env python2

import os
from os.path import join as join
import sys
from wizard_helpers import *
from QFramework import TQLibrary

path = os.path.dirname(os.path.realpath(__file__))

print("Welcome to the TQObservable wizard!")
print("If you are using the wizard for the first time, please get help from the tutorial at https://gitlab.cern.ch/atlas-caf/CAFExample/blob/master/Tutorial/WritingCustomObservables.md.")
print("")

alrbdir = os.environ.get("ATLAS_LOCAL_ROOT_BASE")
if alrbdir:
  print("Found an ASG setup in "+alrbdir)
else:
  print("You do not currently have ASG setup. If you want to add your new observable to a package, you should first setup an ASG release. Please choose:")
  if raw_input("Continue and write files to current working directory (type 'c') or abort (type anything else)? ") == "c":
    pkgname = ""
  else:
    print("aborting!")
    quit()
  pkgdir = os.getcwd()

# check if RootCore setup is used:
isRootCoreSetup = False
if 'ROOTCOREBIN' in os.environ:
    rcdir=os.path.dirname(os.environ['ROOTCOREBIN'])
    # if cmake is used RootCoreBin will be set to an cvmfs path, take care of this:
    if rcdir and not "/cvmfs/atlas.cern.ch/" in rcdir: isRootCoreSetup = True

basedir = getAnalysisPackageBaseDir()
print("Found base directory of your analysis framework setup in "+basedir)

all_packages = TQLibrary.getListOfPackages()
if len(all_packages) < 1:
  print("ERROR: No local packages found! Recompiling might help, otherwise contact experts.")
  print("Aborting Now!")
  quit()
print("Should the observable wizard put the files into your current working directory (leave empty) or into some package (type package name)?")
print("These are your current local packages (querying for packages...): ")
pkg_paths_from_basedir = []
pkg_names = []
for pkg in all_packages: # pkg is just the name of the package invariant in which submodule
  if os.path.exists(join(basedir,"..", pkg, "Root")): # check if the basedir is already the path of pkg
    pkg_paths_from_basedir.append("")
    pkg_names.append(pkg)
    print(pkg+" (located in '.')")
  for root, dirs, files in os.walk(basedir):
    # don't crawl through .git or other hidden directories, RootCoreBin or share folder in analysis package
    # root.split("/")[len(basedir.split("/")):] handles case where
    # folder with name 'share' is already in basedir
    if "/." in root or "RootCoreBin" in root or "share" in root.split("/")[len(basedir.split("/")):]: continue
    for name in dirs:
      if name[0] == ".": continue
      if os.path.exists(join(root, name, pkg)) and os.path.exists(join(root, name, pkg, "Root")):
        pkg_paths_from_basedir.append(join(name,pkg))
        pkg_names.append(pkg)
        print(pkg+" (located in '"+name+")'")
pkgname = raw_input("Type your choice: ")
if (not pkgname in pkg_names) and (not pkgname == ""):
  print("No such package found, aborting!")
  quit()
if not pkgname == "":
  pkgdir_relative = [pkg_paths_from_basedir[i] for i, s in enumerate(pkg_names) if pkgname in s]
  pkgdir = join(basedir,pkgdir_relative[0])
else:
  pkgdir = join(os.getcwd())

# ask for classname (for QFramework classes, new observable class should start with TQ)
classname = ""
if pkgname == "QFramework":
  while not "TQ" in classname:
    classname = raw_input("What is the name of the observable you would like to create? Since you selected QFramework as a target package, the name is required to start with 'TQ'. Enter the name of your observable: ")
else:
  classname = raw_input('What is the name of the observable you would like to create? ')

# ask for observable type (select template file)
print("What type of observable would you like to create?")
print("If you want to read xAODs using the xAOD::TEvent mechanism, please type 'Event'")
print("If you want to access the TTree pointer and use TTreeFormula, please type 'Tree'")
print("If your new observable does not need direct data access but uses some custom mechanism to work, please leave empty.")
type = raw_input("Please specify the type of observable, choose  from {Event,Tree,<empty>}: ")
if type == "Event":
  typename = "TQEventObservable"
elif type == "Tree":
  typename = "TQTreeObservable"
else:
  typename = "TQObservable"

vector_observable = (raw_input("Do you want to create a vector observable that can return multiple values? (y/N) ").lower() == "y")

# ask if expression handling is requested
print("Some observable classes have an 'expression' member variable that allows to alter the configuration based on sample tags, but complicates identifying the right observable.")
include_expression = ( raw_input("Should your class have an 'expression' member variable? (y/N) ").lower() == "y")
advanced_expression = False
if include_expression:
  advanced_expression = ( raw_input("Does your class rely on advanced string parsing capabilities for the 'expression' member variable? (y/N) ").lower() == "y")

# check for factory
has_factory = ( raw_input("Are you planning to provide a factory for your observable class? (y/N)").lower() == "y")

# print summary and ask for confirmation
print("Your choices:")
print("class name: " + classname)
print("inherits from: " + typename)
print("in package: " + pkgname)
print("written to directory: " + pkgdir)
if include_expression:
  if advanced_expression:
    print("including advanced expression member")
  else:
    print("including expression member")
else:
  print("not including expression member")
if has_factory:
  print("configured for factory use")
build = raw_input("build this observable now? (y/N) ")
if not build.lower() == "y":
  print("aborted")
  quit()

# return header and source file
hcode, cxxcode = patchHeaderAndSourceFile(path, pkgname, classname, typename, vector_observable, include_expression, advanced_expression, has_factory)

# write the new source files to the target directory
if pkgname:
  cxxfile = join(pkgdir,"Root",classname+".cxx")
  hfile   = join(pkgdir,pkgname,classname+".h")
else:
  cxxfile = join(pkgdir,classname+".cxx")
  hfile   = join(pkgdir,classname+".h")
writefile(hfile,hcode)
writefile(cxxfile,cxxcode)

# look for a linkdeffile, ask the user if he/she wants us to patch it (Only needed with RootCore setup)
if isRootCoreSetup:
  linkdeffile = join(pkgdir,"Root","LinkDef.h")
  if os.path.exists(linkdeffile):
    if raw_input("the wizard has detected a LinkDef.h file in the target package - do you want to add an entry for your observable class? (y/N) ").lower() != "n":
      makeLinkDefEntry(linkdeffile, pkgname, classname)

print "Your observable skeleton was successfully created. The created files can be found in"
print " "+join(pkgdir,pkgname,classname+".h")
print " "+join(pkgdir,"Root",classname+".cxx")
print "After configuring the observable as desired, don't forget to do the following: "
print "- Execute cmake and recompile (calling the alias 'cafcompile' takes care of both)!"
print "- Add a python snippet in the appropriate observables/ folder in your analysis package!"
# print "to execute cmake and to recompile to add the observable to your setup (If you have the alias 'cafcompile' it will take care of both)"
