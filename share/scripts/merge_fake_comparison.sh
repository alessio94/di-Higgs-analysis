#!/usr/bin/env bash

for version in lep_1_pt lep_1_pt_channelsplit lep_1_pt_coarse_channelsplit; do
    echo ./merge.sh bbll_fakes_${version}
    ./merge.sh bbll_fakes_${version}
done
