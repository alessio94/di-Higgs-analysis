#!/bin/bash

mkdir -p data/ntup/merged/hhcombcrosscheck

hadd -f data/ntup/merged/hhcombcrosscheck/bbll_MC.root data/ntup/unmerged/hhcombcrosscheck/hhcombcrosscheck_{sig,sys,oth}*.root
hadd -f data/ntup/merged/hhcombcrosscheck/bbll_DATA.root data/ntup/unmerged/hhcombcrosscheck/hhcombcrosscheck_data*.root
