#!/bin/bash

for id in "Medium" "Tight"; do
  for iso in "FixedCutLoose" "Gradient"; do
    identifier="ele_${id}_${iso}"
    echo tqmerge -o "sampleFolders/analyzed/samples-analyzed-object_defs-${identifier}.root" -s samples -t analyze "batchOutput/unmerged_${identifier}/*root"
    tqmerge -o "sampleFolders/analyzed/samples-analyzed-object_defs-${identifier}.root" -s samples -t analyze "batchOutput/unmerged_${identifier}"/*root
  done
done
