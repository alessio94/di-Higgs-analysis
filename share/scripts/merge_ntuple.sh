#!/bin/bash

mkdir -p data/ntup/merged
for tag in 1tag 2tag; do
  for region in sr1 sr2 topcr; do
    hadd -f data/ntup/merged/${tag}_${region}_sig.root data/ntup/unmerged/mva_${tag}_${region}_ntuple_sig_*
    hadd -f data/ntup/merged/${tag}_${region}_bkg.root data/ntup/unmerged/mva_${tag}_${region}_ntuple_bkg_*
    hadd -f data/ntup/merged/${tag}_${region}_data.root data/ntup/unmerged/mva_${tag}_${region}_ntuple_data_*
  done
done
