#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
This script generates a cutflow style file, i.e. the file in which the order of the
cuts is specified for the cutflow in readAnalysis.py. The order of the cuts is taken
from the titles in the cutfile.

Usage:
    > generate_cutflow_style.py inputfile outputfile

    with
     - inputfile: cutfile
     - outputfile: cutflow style file
"""

import argparse
import re


def main(args):
    regex = re.compile(r"\+(\w*)")
    cut_names = list()

    with open(args.inputfile) as f:
        for line in f.readlines():
            match = regex.search(line)
            if match:
                cut_names.append(match.group(1))
            elif line.startswith("# CF:"):
                command = line.split("# CF:")[1].strip()
                if command in ["|", "||"]:
                    cut_names.append(command)
                else:
                    print("Command '{}' not recognized!".format(command))

    with open(args.outputfile, "w") as f:
        for cut_name in cut_names:
            f.write('.name = "{}"\n'.format(cut_name))


if __name__ == "__main__":
    PARSER = argparse.ArgumentParser(description="Create cutflow style file with correct order")
    PARSER.add_argument("inputfile", help="cutfile")
    PARSER.add_argument("outputfile", help="cutflow style file")
    main(PARSER.parse_args())
