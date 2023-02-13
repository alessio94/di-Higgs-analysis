#!/bin/bash

mkdir -p data/ntup/merged/mva

for tag in 1tag 2tag; do
  for region in sr1 sr2 topcr; do
    hadd -f data/ntup/merged/mva/${tag}_${region}.root \
      data/ntup/unmerged/mva_${tag}_${region}_ntuple_sig_* \
      data/ntup/unmerged/mva_${tag}_${region}_ntuple_bkg_*
      #data/ntup/unmerged/mva_${tag}_${region}_ntuple_data_*
  done
done
