#!/bin/bash

for region in SR1VBFVeto SR2 TopCR ZllCR; do
  mkdir -p data/ntup/merged/stat-only/${region}
  hadd -f data/ntup/merged/stat-only/${region}/bbll.root \
    data/ntup/unmerged/stat-only/stat-only_sig_*${region}* \
    data/ntup/unmerged/stat-only/stat-only_bkg_*${region}* \
    data/ntup/unmerged/stat-only/stat-only_data_*${region}* \
    data/ntup/unmerged/stat-only/stat-only_sys_*${region}*
done
