#!/bin/bash

for iso in "PflowLoose_VarRad" "PflowLoose_FixedRad" "PflowTight_VarRad" "PflowTight_FixedRad"; do
    identifier="mu_${iso}"
    echo tqmerge -o "sampleFolders/analyzed/samples-analyzed-object_defs-${identifier}.root" -s samples -t analyze "batchOutput/unmerged_${identifier}/*root"
    tqmerge -o "sampleFolders/analyzed/samples-analyzed-object_defs-${identifier}.root" -s samples -t analyze "batchOutput/unmerged_${identifier}"/*root
done
