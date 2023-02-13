#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import argparse


def main(args):
    infile = TFile.Open(args.input_file, "READ")
    ttree = xAOD.MakeTransientTree(infile, "CollectionTree")
    TQUtils.printBranches(ttree)
    ROOT.xAOD.ClearTransientTrees()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Print branches in an xAOD")
    parser.add_argument("input_file", type=str, help="Name of input file")
    args = parser.parse_args()

    from QFramework import *
    from ROOT import *

    main(args)
