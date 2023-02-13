#!/bin/bash
# this is an auto-generated setup script
export CAFANALYSISSETUP=/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/cafsetup.sh
export CAFCOREDIR=/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore
export CAFBUILDDIR=/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build
export TQPATH=/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework
export PATH=${PATH}:${TQPATH}/share:/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CommonAnalysisHelpers/share:/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/SFramework/share

if [ "$(find /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCorePluginSetup -name '*.sh' -print -quit 2>/dev/null)" ]; then
for setupscript in /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCorePluginSetup/*.sh; do source ${setupscript}; done
fi
function cafbuild(){
    local nProcessors=$(($(nproc)-1))
    if [[ "$nProcessors" -lt 1 ]]; then
        nProcessors="1"
    fi
    cd /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build && make -j ${CAF_BUILD_CORES:-$nProcessors}
    local retVal=$?
    cd -
    return $retVal
}
function cafcompile(){
    cd /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build && cmake /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll
    local retVal=$?
    cd -
    if [[ retVal -gt 0 ]]; then
        return $retVal
    fi
    cafbuild
    retVal=$?
    return $retVal
}

alias cafdiagnostics='bash /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/cafdiagnostics.sh 2>&1 | tee cafdiagnostics.txt; echo Wrote diagnostics output to $PWD/cafdiagnostics.txt'
alias tqpy='python -i $TQPATH/share/tqpy'
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/lib
export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/lib
export CAF_LIBRARIES=${CAF_LIBRARIES}:/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/lib
export ROOT_INCLUDE_PATH=${ROOT_INCLUDE_PATH}:/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/include
export PYTHONPATH=${PYTHONPATH}:/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore:/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python:/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/lib
export LIBXMLPATH=/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/lib/libxml2.so
