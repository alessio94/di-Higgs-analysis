#!/bin/bash

echo "submit.py bbll/config/master/analyze-btagging.cfg --jobs bbll/config/jobLists/default.txt --allowArgChanges --identifier btagging" "$@"
submit.py bbll/config/master/analyze-btagging.cfg --jobs bbll/config/jobLists/default.txt --allowArgChanges --identifier btagging "$@"
