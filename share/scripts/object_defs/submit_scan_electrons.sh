#!/bin/bash

for id in "Medium" "Tight"; do
  for iso in "FixedCutLoose" "Gradient"; do
    echo "submit.py bbll/config/master/analyze.cfg --jobs bbll/config/jobLists/default.txt --allowArgChanges --identifier ele_${id}_${iso} --options electrons.Id=${id} electrons.Iso=${iso}" "$@"
    submit.py bbll/config/master/analyze.cfg --jobs bbll/config/jobLists/default.txt --allowArgChanges --identifier "ele_${id}_${iso}" --options electrons.Id="${id}" electrons.Iso="${iso}" "$@"
  done
done
