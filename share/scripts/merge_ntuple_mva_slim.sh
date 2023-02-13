#!/bin/bash

mkdir -p data/ntup/merged/mva_slim

#for tag in 1tag 2tag; do
for region in sr1 sr2 topcr zllcr; do
  hadd -f data/ntup/merged/mva_slim/2tag_${region}.root \
    data/ntup/unmerged/mva_slim/mva_2tag_${region}_ntuple_slim_sig_* \
    data/ntup/unmerged/mva_slim/mva_2tag_${region}_ntuple_slim_bkg_*
    #data/ntup/unmerged/mva_slim/mva_2tag_${region}_ntuple_slim_data_*
done
