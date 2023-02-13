#!/bin/bash

set -euo pipefail

input_dir="$1"

start_pwd="$PWD"
cd "$CAFANALYSISBASE/klambdareweighttool/scripts"

for input_file in $(ls "$start_pwd/$input_dir"); do
    input_file_path="$start_pwd/$input_dir/$input_file"
    echo $input_file_path

    for kl_point in $(seq -30.0 0.2 30.0); do
        kl_point_formatted=$(echo $kl_point | sed "s/-/n/" | sed "s/\./p/g")
        if [ "$kl_point_formatted" = "1p0" ]; then
            continue
        fi
        root -b -q -l "hhReweightAnalysisLvl.C(\"$input_file_path\", \"truth_mhh\", \"$kl_point_formatted\", \"10\", \"01d0\", 1, 1)"
    done
done

cd "$start_pwd"
