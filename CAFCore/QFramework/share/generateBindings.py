#!/bin/env python

import os
import errno

def ensure_dir(directory):
    """Create (parent) directories if non-existent"""
    try:
        os.makedirs(directory)
    except OSError as e:
        if e.errno != errno.EEXIST:
            raise


def fhash(fname):
    # get a hash of a file (using stupid filesize replacement for now)
    import os
    statinfo = os.stat(fname)
    return statinfo.st_size

def gettimehash(path):
    # get a modification date and hash of a file 
    from os.path import getmtime,isfile
    if not isfile(path): return (None,None)
    return(getmtime(path),fhash(path))

def uptimehash(path,oldtime,oldhash):
    # reset timestamp of file if hash did not change
    if oldhash==None or oldtime==None: return
    newtime,newhash=gettimehash(path)
    from os import utime
    if oldhash==newhash:
        utime(path,(oldtime,oldtime))

# helper class to allow conditional "with" wrapping
class Dummysink(object):
    def write(self, data):
        pass # ignore the data
    def __enter__(self): return self
    def __exit__(*x): pass

def datasink(filename):
    if filename:
        directory = os.path.dirname(filename)
        ensure_dir(directory)
        return open(filename, "wt")
    else:
        return Dummysink()

def isdummy(sink):
    return isinstance(sink,Dummysink)


# main function doing all the work
def main(args):
    # some text parsing
    import re
    from os.path import basename,isfile,splitext
    from os.path import join as pjoin
    classre = re.compile("^[ ]*class[ ]*([A-Za-z0-9_]+)[ ]*[:{]")
    typedefre = re.compile("^[ ]*typedef[ ]*[A-Za-z0-9<>:_,*]*[ ]*([A-Za-z0-9_]+)[ ]*.*")
    namespacere = re.compile("^[ ]*namespace[ ]*([A-Za-z0-9]+)[ ]*[:{]")
    nestedre = re.compile(".*//[ ]*nested")
    excludere = re.compile(".*//[ ]*EXCLUDE")

    pytime,pyhash,ldtime,ldhash=None,None,None,None
    if args.python and isfile(args.python):
        pytime,pyhash=gettimehash(args.python)
    if args.linkdef and isfile(args.linkdef):
        ldtime,ldhash=gettimehash(args.linkdef)

    with datasink(pjoin(args.wd,args.linkdef)) as linkdef:
        with datasink(pjoin(args.wd,args.python)) as python:
            # generate the linkdef header
            if args.verbose and not isdummy(linkdef):
                print("Generating LinkDef.h")
            linkdef.write("//this is an automatically generated -*- c++ -*- file - EDITS WILL BE LOST!\n")
            linkdef.write("\n")
            linkdef.write("#ifndef __"+args.pkgname+"DICT__\n")
            linkdef.write("#define __"+args.pkgname+"DICT__\n")
            linkdef.write("\n")
            linkdef.write("#pragma GCC diagnostic push\n")
            linkdef.write('#pragma GCC diagnostic ignored "-Winconsistent-missing-override"\n')
            linkdef.write("\n")
            for hfile in args.headers:
                linkdef.write('#include "'+pjoin(args.pkgname,basename(hfile))+'"\n')
            linkdef.write("\n")
            linkdef.write("#ifdef __CINT__\n")
            linkdef.write("\n")
            linkdef.write("#pragma link off all globals;\n")
            linkdef.write("#pragma link off all classes;\n")
            linkdef.write("#pragma link off all functions;\n")
            linkdef.write("#pragma link C++ nestedclass;\n")
            linkdef.write("#pragma link C++ nestedtypedef;\n")
        
            # generate the python header
            if args.verbose and not isdummy(python):
                print("Generating __init__.py")
            python.write("# this is an automatically generated -*- python -*- file - EDITS WILL BE LOST!\n")
            if args.loadpackage:
                python.write("\n")
                python.write("import ROOT\nROOT.gSystem.Load(\"lib"+args.pkgname+".so\")")
                python.write("\n")
            if args.pythonhead:
                python.write("\n\n")
                python.write("################################################\n")
                python.write("###         begin of imported section        ###\n")
                python.write("################################################\n")
                python.write("\n\n")
                with open(pjoin(args.wd,args.pythonhead),"rt") as inpython:
                    for line in inpython:
                        python.write(line)

            python.write("\n\n")
            python.write("################################################\n")
            python.write("### begin of automatically generated section ###\n")
            python.write("################################################\n")
            python.write("\n\n")

            # loop over all  header files, collecting class definition, namespace definition and typedef lines
            for hfile in args.headers:
                linkdef.write("// >>> begin "+basename(hfile)+"\n")
                with open(pjoin(args.wd,hfile),"rt") as header:
                    current_line = ""
                    for line in header:
                        current_line += line.strip()
                        # if class definition goes over multiple lines collect complete definition first
                        if current_line.startswith("class") and ":" not in current_line and "{" not in current_line and not current_line.endswith(";"):
                            continue
                        classmatch = classre.match(current_line)
                        namespacematch = namespacere.match(current_line)
                        typedefmatch = typedefre.match(current_line)
                        # write the #pragma lines
                        if classmatch and not nestedre.match(current_line):
                            classname = classmatch.group(1)
                            linkdef.write("#pragma link C++ class "+classname+"+;\n")
                            python.write(classname+"=ROOT."+classname+"\n")
                        if typedefmatch and not excludere.match(current_line):
                            linkdef.write("#pragma link C++ typedef "+typedefmatch.group(1)+";\n")
                        if namespacematch and not excludere.match(current_line):
                            namespacename = namespacematch.group(1)
                            linkdef.write("#pragma link C++ namespace "+namespacename+";\n")
                            python.write(namespacename+"=ROOT."+namespacename+"\n")
                        current_line = ""
                # import additional linkdef code
                addfiles = [ pjoin(args.wd,splitext(hfile)[0]+"LinkDef.h.add") ]
                for addfile in addfiles:
                    if isfile(addfile):
                        with open(addfile,"rt") as afile:
                            for line in afile:
                                linkdef.write(line)
                linkdef.write("// <<< end "+basename(hfile)+"\n")
                linkdef.write("\n")
        
            # write the linkdef footer
            linkdef.write("\n")
            linkdef.write("#endif //__CINT__\n")
            linkdef.write("#endif //__"+args.pkgname+"DICT__\n")

    if args.python:
        uptimehash(args.python,pytime,pyhash)
    if args.linkdef:
        uptimehash(args.linkdef,ldtime,ldhash)


if __name__ == "__main__":
    # parse the CLI arguments
    from argparse import ArgumentParser
    parser = ArgumentParser(description='auto-generate a LinkDef.h file from a set of header files')
    parser.add_argument('--linkdef', metavar='LinkDef.h', type=str, help='path of output LinkDef file')
    parser.add_argument('--python-head', dest="pythonhead", metavar='head.py', type=str, help='header of python bindings')
    parser.add_argument('--python', metavar='__init__.py', type=str, help='path of output python file')
    parser.add_argument('--pkgname', metavar='PKGNAME', type=str, help='name of the package', required=True)
    parser.add_argument('--headers', nargs='+', metavar='HEADER', type=str, help='input files', required=True)
    parser.add_argument('--verbose', action='store_true')
    parser.add_argument('--set-working-directory', metavar='/where/you/are', dest="wd", type=str, help='working directory',required=True)
    parser.add_argument('--loadpackage', action='store_true')
    args = parser.parse_args()
    main(args)
