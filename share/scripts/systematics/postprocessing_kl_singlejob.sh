#!/bin/bash

set -euo pipefail

input_file="$1"
output_file="$2"
kl_point_formatted="$3"

start_pwd="$PWD"
cd "$CAFANALYSISBASE/klambdareweighttool/scripts"

root -b -q -l "hhReweightAnalysisLvl.C(\"$input_file\", \"truth_mhh\", \"$kl_point_formatted\", \"10\", \"01d0\", 1, 1)"

cd "$start_pwd"
touch $output_file
