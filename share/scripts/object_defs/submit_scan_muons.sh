#!/bin/bash

for iso in "PflowLoose_VarRad" "PflowLoose_FixedRad" "PflowTight_VarRad" "PflowTight_FixedRad"; do
    echo "submit.py bbll/config/master/analyze.cfg --jobs bbll/config/jobLists/default.txt --allowArgChanges --identifier mu_${iso} --options muons.Iso=${iso}" "$@"
    submit.py bbll/config/master/analyze.cfg --jobs bbll/config/jobLists/default.txt --allowArgChanges --identifier "mu_${iso}" --options muons.Iso="${iso}" "$@"
done
