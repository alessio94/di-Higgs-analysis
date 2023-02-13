#!/bin/bash

CAFCopyPlottingFiles(){
    # These commands copy the output files of the plotter into the
    # directory plotting-artifacts, from where they are uploaded to
    # gitlab.

    local fromPath="$1"
    local toPathRaw="$2"
    local toPath="$toPathRaw/plotting-artifacts"

    if [ -d "$toPath" ]; then
	rm -r "$toPath"
    fi
    mkdir "$toPath"

    if [ -f "$fromPath/messageForGitlab.txt" ]; then
	# if messageForGitlab.txt exists, then plots don't need to be duplicated
	mv "$fromPath/messageForGitlab.txt" "$toPath/README.txt"
    else
	# if messageForGitlab.txt does not exist, upload all plots
	if [ -d "$fromPath/dotCFiles" ]; then
	    mv "$fromPath/dotCFiles" "$toPath/referencePlots"
	fi
	if [ -d "$fromPath/pdfFiles" ]; then
	    mv "$fromPath/pdfFiles" "$toPath/referencePlotsPDF"
	fi
	if [ "$fromPath" != "$toPath" ]; then
	    if [ -f "$fromPath/listOfHashes.txt" ]; then
		mv "$fromPath/listOfHashes.txt" "$toPath"
	    fi
	    if [ -f "$fromPath/fullDiff.txt" ]; then
		mv "$fromPath/fullDiff.txt" "$toPath"
	    fi
	fi
    fi
}

CAFRunCMakeTests(){
    set +e

    local PREV_DIR=$PWD
    cd $CAFBUILDDIR
    make test ARGS="-V"
    returnCode=$?

    CAFCopyPlottingFiles "$CAFBUILDDIR/QFramework/unitTestRun" "/builds/atlas-caf/CAFCore/"

    cd $PREV_DIR
    set -e
    return $returnCode
}

CAFRunCMakeStandaloneTests(){
    set +e

    local PREV_DIR=$PWD
    cd $CAFBUILDDIR
    make test ARGS="-V"
    returnCode=$?

    CAFCopyPlottingFiles "$CAFBUILDDIR/QFramework" "/builds/atlas-caf/CAFCore/"

    cd $PREV_DIR
    set -e
    return $returnCode
}
