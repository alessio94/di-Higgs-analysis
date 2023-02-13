#!/bin/bash

set -euo pipefail

input_file="$1"
output_file="$2"

start_pwd="$PWD"
cd "$CAFANALYSISBASE/klambdareweighttool/scripts"

for kl_point in $(seq -15 0.2 20); do
    kl_point_formatted=$(echo $kl_point | sed "s/-/n/" | sed "s/\./p/g")
    if [ "$kl_point_formatted" = "1p0" ]; then
        continue
    fi
    root -b -q -l "hhReweightAnalysisLvl.C(\"$input_file\", \"truth_mhh\", \"$kl_point_formatted\", \"10\", \"01d0\", 1, 1)"
done

cd "$start_pwd"
touch $output_file
