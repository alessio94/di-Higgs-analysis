#!/usr/bin/env bash

for version in lep_1_pt lep_1_pt_channelsplit lep_1_pt_coarse_channelsplit; do
    echo submit.py bbll/config/master/analyze-fakes_${version}.cfg --jobs bbll/config/jobLists/ddFake_comparison.txt --allowArgChanges --identifier bbll_fakes_${version} --queue nemo_vm_atlsch "$@"
    submit.py bbll/config/master/analyze-fakes_${version}.cfg --jobs bbll/config/jobLists/ddFake_comparison.txt --allowArgChanges --identifier bbll_fakes_${version} --queue nemo_vm_atlsch "$@"
done
