#!/bin/bash

echo "submit.py bbll/config/master/1tag_res/analyze-systematics-weight.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight $@"
submit.py bbll/config/master/1tag_res/analyze-systematics-weight.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight "$@"

echo "submit.py bbll/config/master/1tag_res/analyze-systematics-weight-signal_py8.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-signal_py8.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight $@"
submit.py bbll/config/master/1tag_res/analyze-systematics-weight-signal_py8.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-signal_py8.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight "$@"

echo "submit.py bbll/config/master/1tag_res/analyze-systematics-weight-signal_vbf.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-signal_vbf.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight $@"
submit.py bbll/config/master/1tag_res/analyze-systematics-weight-signal_vbf.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-signal_vbf.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight "$@"

# this includes all samples with Zee (main) like weight indices: Zee (main), Zee/Zmm verylowmass, Zee/Zmm EWK
echo "submit.py bbll/config/master/1tag_res/analyze-systematics-weight-Zee.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-Zee.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight $@"
submit.py bbll/config/master/1tag_res/analyze-systematics-weight-Zee.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-Zee.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight "$@"

# Zmm (main) -> only scale and alternative PDF weights
echo "submit.py bbll/config/master/1tag_res/analyze-systematics-weight-Zmm.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-Zmm.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight $@"
submit.py bbll/config/master/1tag_res/analyze-systematics-weight-Zmm.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-Zmm.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight "$@"

# Zee/Zmm lowmass -> no systematic weight indices available
echo "submit.py bbll/config/master/1tag_res/analyze-systematics-weight-ZllLM.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-ZllLM.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight $@"
submit.py bbll/config/master/1tag_res/analyze-systematics-weight-ZllLM.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-ZllLM.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight "$@"

echo "submit.py bbll/config/master/1tag_res/analyze-systematics-weight-ttbar.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-ttbar.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight $@"
submit.py bbll/config/master/1tag_res/analyze-systematics-weight-ttbar.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-ttbar.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight "$@"

echo "submit.py bbll/config/master/1tag_res/analyze-systematics-weight-stop.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-stop.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight $@"
submit.py bbll/config/master/1tag_res/analyze-systematics-weight-stop.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-stop.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight "$@"

echo "submit.py bbll/config/master/1tag_res/analyze-systematics-weight-ddfakes.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-ddfakes.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight $@"
submit.py bbll/config/master/1tag_res/analyze-systematics-weight-ddfakes.cfg --jobs bbll/config/jobLists/1tag_res/systematics-weight-ddfakes.txt --allowArgChanges --identifier bbll_systematics_1tag_res_weight "$@"

echo "submit.py bbll/config/master/1tag_res/analyze-systematics-p4.cfg --jobs bbll/config/jobLists/1tag_res/systematics-p4.txt --allowArgChanges --identifier bbll_systematics_1tag_res_p4 $@"
submit.py bbll/config/master/1tag_res/analyze-systematics-p4.cfg --jobs bbll/config/jobLists/1tag_res/systematics-p4.txt --allowArgChanges --identifier bbll_systematics_1tag_res_p4 "$@"

echo "submit.py bbll/config/master/1tag_res/analyze-systematics-p4-zll.cfg --jobs bbll/config/jobLists/1tag_res/systematics-p4-zll.txt --allowArgChanges --identifier bbll_systematics_1tag_res_p4 $@"
submit.py bbll/config/master/1tag_res/analyze-systematics-p4-zll.cfg --jobs bbll/config/jobLists/1tag_res/systematics-p4-zll.txt --allowArgChanges --identifier bbll_systematics_1tag_res_p4 "$@"

echo "submit.py bbll/config/master/1tag_res/analyze-systematics-alternative.cfg --jobs bbll/config/jobLists/1tag_res/systematics-alternative.txt --allowArgChanges --identifier bbll_systematics_1tag_res_alternative $@"
submit.py bbll/config/master/1tag_res/analyze-systematics-alternative.cfg --jobs bbll/config/jobLists/1tag_res/systematics-alternative.txt --allowArgChanges --identifier bbll_systematics_1tag_res_alternative "$@"

echo "submit.py bbll/config/master/1tag_res/analyze-systematics-alternative-signal.cfg --jobs bbll/config/jobLists/1tag_res/systematics-alternative-signal.txt --allowArgChanges --identifier bbll_systematics_1tag_res_alternative $@"
submit.py bbll/config/master/1tag_res/analyze-systematics-alternative-signal.cfg --jobs bbll/config/jobLists/1tag_res/systematics-alternative-signal.txt --allowArgChanges --identifier bbll_systematics_1tag_res_alternative "$@"

echo "submit.py bbll/config/master/1tag_res/analyze-systematics-alternative-zll.cfg --jobs bbll/config/jobLists/1tag_res/systematics-alternative-zll.txt --allowArgChanges --identifier bbll_systematics_1tag_res_alternative $@"
submit.py bbll/config/master/1tag_res/analyze-systematics-alternative-zll.cfg --jobs bbll/config/jobLists/1tag_res/systematics-alternative-zll.txt --allowArgChanges --identifier bbll_systematics_1tag_res_alternative "$@"
