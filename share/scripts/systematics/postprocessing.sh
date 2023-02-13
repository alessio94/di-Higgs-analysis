#!/bin/bash

base_output_dir="data/ntup/merged/systematics"

for region in SR1VBFVeto SR2 TopCR ZllCR; do
    # Zll HF
    for sample in Zee_hf_main_l Zee_hf_main_c Zee_hf_main_b Zee_hf_lowmass_lc Zee_hf_lowmass_b Zee_hf_verylowmass Zee_hf_ew Zmm_hf_main_l Zmm_hf_main_c Zmm_hf_main_b Zmm_hf_lowmass_lc Zmm_hf_lowmass_b Zmm_hf_verylowmass Zmm_hf_ew; do
        echo "Add Z+jets CKKW and QSF weights to sample ${sample} in region ${region}"
        echo ./scripts/systematics/AddZJetsWeights.py "${base_output_dir}/${region}/${sample}.root" --treename ${sample} --mcID dsid --nTruthJets n_truth_jets
        ./scripts/systematics/AddZJetsWeights.py "${base_output_dir}/${region}/${sample}.root" --treename ${sample} --mcID dsid --nTruthJets n_truth_jets
    done;
    # Zll LM
    for sample in Zee_lf_main_l Zee_lf_main_c Zee_lf_main_b Zee_lf_lowmass_lc Zee_lf_lowmass_b Zee_lf_verylowmass Zee_lf_ew Zmm_lf_main_l Zmm_lf_main_c Zmm_lf_main_b Zmm_lf_lowmass_lc Zmm_lf_lowmass_b Zmm_lf_verylowmass Zmm_lf_ew; do
        echo "Add Z+jets CKKW and QSF weights to sample ${sample} in region ${region}"
        echo ./scripts/systematics/AddZJetsWeights.py "${base_output_dir}/${region}/${sample}.root" --treename ${sample} --mcID dsid --nTruthJets n_truth_jets
        ./scripts/systematics/AddZJetsWeights.py "${base_output_dir}/${region}/${sample}.root" --treename ${sample} --mcID dsid --nTruthJets n_truth_jets
    done;
done;
