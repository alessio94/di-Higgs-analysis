#!/bin/bash

mkdir -p data/ntup/merged/2v_scan

for region in SR1 SR2 TopCR ZllCR; do
  mkdir -p data/ntup/merged/2v_scan/${region}
  hadd -f data/ntup/merged/2v_scan/${region}/bbll.root \
    data/ntup/unmerged/2v_scan/ntuple_2v_scan_sig_*${region}* \
    data/ntup/unmerged/2v_scan/ntuple_2v_scan_bkg_*${region}* \
    data/ntup/unmerged/2v_scan/ntuple_2v_scan_data_*${region}*
done
