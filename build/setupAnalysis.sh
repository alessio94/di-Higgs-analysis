#!/bin/bash
# this is an auto-generated setup script

storeOptions="$-"
export CAFANALYSISBASE="/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll"
export CAFANALYSISSHARE="/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/share"

# Add paths from $CAFANALYSISSHARE to $PATH (while avoiding duplicates)
set +e
for directory in `echo $CAFANALYSISSHARE:$CAFANALYSISBASE/tools:$CAFANALYSISBASE/tools/statistics:$CAFANALYSISBASE/tools/plotComparisons | tr ":" " "` ; do
	# Remove trailing slash
	dir=${directory%/}
	# If $dir is not in $PATH yet, add it at the end.
	echo $PATH | tr ":" "\n" | grep -q $dir
	if [[ $? -ne 0 ]] ; then
		export PATH=$PATH:$dir
	fi
done
if [[ "$storeOptions" == *"e"* ]] ; then
	set -e
fi

shelltype=$(ps -p$$ -ocmd=)
if [[ "$shelltype" == *"bash"* ]] && [ -f /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/setup/setupAutoComplete.sh ]; then
	source /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/setup/setupAutoComplete.sh
fi
if [ -f /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/cafsetup.sh ]; then 
	source /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/cafsetup.sh
fi
if [ -f /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/setup/setupLocal.sh ]; then 
	source /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/setup/setupLocal.sh
fi
if [ -f /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/SubmissionHelpers/setup.sh ]; then 
	source /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/SubmissionHelpers/setup.sh
fi

#export path to this script for easier re-setup, e.g., for batch submission
export PYTHONPATH=$PYTHONPATH:/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build
export CAFANALYSISSETUP=/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/setupAnalysis.sh
export ANALYSIS_SETUP=/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/setupAnalysis.sh
export ANALYSIS_BUILD_DIR=/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build
