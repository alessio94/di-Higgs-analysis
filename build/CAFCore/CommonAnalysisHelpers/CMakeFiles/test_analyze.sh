#!/usr/bin/bash
#
# Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
#
# This script is used by CTest to run the test test_analyze with the correct
# environment setup, and post processing.
#

# Transmit errors:
set -e

# Set up the runtime environment:
source /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/setup.sh

# Turn off xAOD monitoring for the test:
export XAOD_ACCESSTRACER_FRACTION=0

# Set the package name, which may be used by post.sh later on:
export ATLAS_CTEST_PACKAGE=CommonAnalysisHelpers

# Run a possible pre-exec script:
# No pre-exec necessary

# Run the test:
/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/CommonAnalysisHelpers/test-bin/test_analyze.exe 2>&1 | tee test_analyze.log; \
    test ${PIPESTATUS[0]} -eq 0

# Set the test's return code in the variable expected by post.sh:
export testStatus=${PIPESTATUS[0]}

# Put the reference file in place if it exists:
if [ -f /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CommonAnalysisHelpers/share/test_analyze.ref ] &&
    [ "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CommonAnalysisHelpers" != "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/CommonAnalysisHelpers" ]; then
    /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake -E make_directory ../share
    /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake -E create_symlink \
     /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CommonAnalysisHelpers/share/test_analyze.ref ../share/test_analyze.ref
fi

# Run a post-exec script:
if type post.sh >/dev/null 2>&1; then
    post.sh test_analyze ""
fi