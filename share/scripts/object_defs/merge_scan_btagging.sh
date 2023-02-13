#!/bin/bash

echo tqmerge -o "sampleFolders/analyzed/samples-analyzed-object_defs-btagging.root" -s samples -t analyze "batchOutput/unmerged_btagging/*root"
tqmerge -o "sampleFolders/analyzed/samples-analyzed-object_defs-btagging.root" -s samples -t analyze "batchOutput/unmerged_btagging/"*root
