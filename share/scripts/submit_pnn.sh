#!/bin/bash

echo "submit.py bbll/config/master/analyze-stat-only-kl-pnn-0p2.cfg --jobs bbll/config/jobLists/stat-only-kl-pnn.txt --allowArgChanges --identifier stat-only-kl-pnn-0p2 --queue nemo_vm_atlsch $@"
submit.py bbll/config/master/analyze-stat-only-kl-pnn-0p2.cfg --jobs bbll/config/jobLists/stat-only-kl-pnn.txt --allowArgChanges --identifier stat-only-kl-pnn-0p2 --queue nemo_vm_atlsch "$@"

echo "submit.py bbll/config/master/analyze-stat-only-kl-pnn-1p0.cfg --jobs bbll/config/jobLists/stat-only-kl-pnn.txt --allowArgChanges --identifier stat-only-kl-pnn-1p0 --queue nemo_vm_atlsch $@"
submit.py bbll/config/master/analyze-stat-only-kl-pnn-1p0.cfg --jobs bbll/config/jobLists/stat-only-kl-pnn.txt --allowArgChanges --identifier stat-only-kl-pnn-1p0 --queue nemo_vm_atlsch "$@"

echo "submit.py bbll/config/master/analyze-stat-only-kl-pnn-2p0.cfg --jobs bbll/config/jobLists/stat-only-kl-pnn.txt --allowArgChanges --identifier stat-only-kl-pnn-2p0 --queue nemo_vm_atlsch $@"
submit.py bbll/config/master/analyze-stat-only-kl-pnn-2p0.cfg --jobs bbll/config/jobLists/stat-only-kl-pnn.txt --allowArgChanges --identifier stat-only-kl-pnn-2p0 --queue nemo_vm_atlsch "$@"
