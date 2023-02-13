#!/bin/bash

if [ -z "$1" ]
then
  echo "Call the merge script with ./merge.sh <identifier>, where the identifier is the same as used in submitting the jobs to the batch system"
  exit 0
fi

identifier="$1"
echo tqmerge -o "sampleFolders/analyzed/samples-analyzed-${identifier}.root" -s samples -t analyze "batchOutput/unmerged_${identifier}/*root"
tqmerge -o "sampleFolders/analyzed/samples-analyzed-${identifier}.root" -s samples -t analyze "batchOutput/unmerged_${identifier}"/*root
