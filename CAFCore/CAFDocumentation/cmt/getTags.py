#!/bin/env python

import re
import os

path = os.getenv("TQPATH")+"/Root/"

gettag = re.compile('getTag(String|Integer|Double|Bool)[^(]*\("([^"]*)"')
getfunc = re.compile("[ ]*[void|int|TString|std::string|bool]+[ *]+([A-Za-z0-9_]+)::([A-Za-z0-9_]+)[ <>A-Za-z0-9]*\(.*")


for fname in os.listdir(path):
    if not fname.endswith(".cxx"): continue
    with open(path+fname) as f:
        for line in f.readlines():
            if "::" in line:
                func = getfunc.match(line)
                if func:
                    classname = func.group(1)
                    funcname = func.group(2)
            if "getTag" in line:
                match = gettag.search(line)
                if match:
                    tagtype = match.group(1).lower()
                    key = match.group(2)
                    print(fname + ": " + classname + "::" + funcname + ": " + key + " (" + tagtype + ")")


