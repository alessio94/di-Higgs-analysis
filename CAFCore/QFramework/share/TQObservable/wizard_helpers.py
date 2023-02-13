import os
from os.path import join as join
import sys
import fileinput

def getAnalysisPackageBaseDir():
  # Idea: crawl back script_path to dir where no CMakeLists.txt file is defined anymore. Then we are one dir below the base directory of the analysis package
  script_path = sys.path[0]
  dir_backwards = [".."]
  # First crawl back until we find a CMakeLists.txt file defined!
  while not os.path.exists(join(script_path, dir_backwards[-1], "CMakeLists.txt")):
    dir_backwards.append(join(dir_backwards[-1], ".."))
  # Now we know where the 'first' CMakeLists file is and can crawl further asking when there is no CMakeLists file anymore
  while os.path.exists(join(script_path, dir_backwards[-1], "CMakeLists.txt")):
    dir_backwards.append(join(dir_backwards[-1], ".."))
  basedir = join(script_path, dir_backwards[-2])
  return os.path.normpath(basedir) # normpath to get rid of "/../../" in filepath (nicer to read)

# search and replace the classname in the template files
def searchandreplace(code, pkgname, classname, typename):
  code = code.replace("ObsName",classname).replace("OBSNAME",classname.upper()).replace("ParentClass",typename)
  if pkgname:
    code = code.replace("PkgName",pkgname)
  else:
    code = code.replace("PkgName/","")
  return code

# write a modified template file
def writefile(fname,code):
  if os.path.exists(fname) and not raw_input("file '"+fname+"' exists - overwrite? (y/N) ").lower() == "y":
    print("aborted")
    quit()
  else:
    f = open(fname, 'w')
    f.write(code)
    f.close()
    print("wrote '"+fname+"'!")

# read the contents of a file to a string
def readfile(fname):
  with open(fname,"r") as f:
    return f.read()

def patchHeaderAndSourceFile(path, pkgname, classname, typename, vector_observable, include_expression, advanced_expression, has_factory):
  # open the header files and patch them together
  if vector_observable: specific_typename = typename+"_VectorMode"
  else: specific_typename = typename
  hcode =readfile(join(path,"header","head.h"))
  hcode = hcode + readfile(join(path,"header",specific_typename+".h"))
  hcode = hcode + readfile(join(path,"header","initializefinalize.h"))
  if include_expression:
    hcode = hcode + readfile(join(path,"header","expression.h"))
    if advanced_expression:
      hcode = hcode + readfile(join(path,"header","advancedexpression.h"))
  else:
    hcode = hcode + readfile(join(path,"header","noexpression.h"))
  if has_factory:
    hcode = hcode + readfile(join(path,"header","factory.h"))
  else:
    hcode = hcode + readfile(join(path,"header","nofactory.h"))
  hcode = hcode + readfile(join(path,"header","foot.h"))
  hcode = searchandreplace(hcode, pkgname, classname, typename)

  cxxcode =readfile(join(path,"implementation","head.cxx"))
  cxxcode = cxxcode + readfile(join(path,"implementation",specific_typename+".cxx"))
  if not advanced_expression:
    if type == "Tree":
      if not vector_observable:
        cxxcode = cxxcode + readfile(join(path,"implementation","initializefinalize.tree.cxx"))
      else:
        cxxcode = cxxcode + readfile(join(path,"implementation","initializefinalize.tree_vector.cxx"))
    else:
      if not vector_observable:
        cxxcode = cxxcode + readfile(join(path,"implementation","initializefinalize.event.cxx"))
      else:
        cxxcode = cxxcode + readfile(join(path,"implementation","initializefinalize.event_vector.cxx"))
      
  if include_expression:
    cxxcode = cxxcode + readfile(join(path,"implementation","expression.cxx"))
    if advanced_expression:
      cxxcode = cxxcode + readfile(join(path,"implementation","advancedexpression.cxx"))
  else:
    cxxcode = cxxcode + readfile(join(path,"implementation","noexpression.cxx"))
  if has_factory:
    cxxcode = cxxcode + readfile(join(path,"implementation","factory.cxx"))
  cxxcode = searchandreplace(cxxcode, pkgname, classname, typename)

  return hcode, cxxcode

def makeLinkDefEntry(linkdeffile, pkgname, classname):
  watch_include = False
  watch_pragma = False
  inclstr = '#include "'+join(pkgname,classname)+'.h"'
  pragmastr = '#pragma link C++ class '+classname+"+;"
  # check if it's already patched
  with open(linkdeffile, 'r') as f:
    if any(pragmastr == x.rstrip('\r\n') for x in f):
      done_pragma = True
    else:
      done_pragma = False
    if any(inclstr == x.rstrip('\r\n') for x in f):
      done_include = True
    else:
      done_include = False
      # search for the 'right' place to insert it
  for line in fileinput.input(linkdeffile, inplace=1):
    if not done_include and line.lower().startswith('#include'):
      watch_include = True
    if watch_include and not line.lower().startswith('#include'):
      print(inclstr)
      watch_include = False
      done_include = True
    if not done_pragma and line.lower().startswith('#pragma link c++ class'):
      watch_pragma = True
    if watch_pragma and not line.lower().startswith('#pragma link c++ class'):
      print(pragmastr)
      watch_pragma = False
      done_pragma = True
    print(line.strip())
