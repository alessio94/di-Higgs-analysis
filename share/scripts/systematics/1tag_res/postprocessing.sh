#!/bin/bash

base_output_dir="data/ntup/merged/systematics"

for region in 2tagSR1 2tagSR2 2tagTopCR 2tagZllCR 1tagSR1 1tagSR2 1tagTopCR; do
    for sample in Zee_main_l Zee_main_c Zee_main_b Zmm_main_l Zmm_main_c Zmm_main_b; do
        echo "Add Z+jets CKKW and QSF weights to sample ${sample} in region ${region}"
        echo ./scripts/systematics/AddZJetsWeights.py "${base_output_dir}/${region}/${sample}.root" --treename ${sample} --mcID dsid --nTruthJets n_truth_jets
        ./scripts/systematics/AddZJetsWeights.py "${base_output_dir}/${region}/${sample}.root" --treename ${sample} --mcID dsid --nTruthJets n_truth_jets
    done;
done;
