#!/bin/bash

echo "submit.py bbll/config/master/analyze_emtopo.cfg --jobs bbll/config/jobLists/emtopo_pflow.txt --allowArgChanges --identifier emtopo" "$@"
submit.py bbll/config/master/analyze_emtopo.cfg --jobs bbll/config/jobLists/emtopo_pflow.txt --allowArgChanges --identifier emtopo "$@"

echo "submit.py bbll/config/master/analyze_pflow.cfg --jobs bbll/config/jobLists/emtopo_pflow.txt --allowArgChanges --identifier pflow" "$@"
submit.py bbll/config/master/analyze_pflow.cfg --jobs bbll/config/jobLists/emtopo_pflow.txt --allowArgChanges --identifier pflow "$@"
