set -e
tmpdir=`mktemp -d makeCalcGenericMT2LibCintDict.XXXXXX`
cd ${tmpdir}
/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/bin/rootcling -f CalcGenericMT2LibCintDict.cxx -s libCalcGenericMT2Lib.so -rml libCalcGenericMT2Lib.so -rmf CalcGenericMT2Lib.dsomap -DHAVE_PRETTY_FUNCTION -DHAVE_64_BITS -D__IDENTIFIER_64BIT__ -DATLAS -DROOTCORE -DXAOD_STANDALONE -DXAOD_ANALYSIS -DROOTCORE_RELEASE_SERIES=25 -DPACKAGE_VERSION=\"CalcGenericMT2-00-00-01\" -DPACKAGE_VERSION_UQ=CalcGenericMT2-00-00-01 -I"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CalcGenericMT2" -I"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CalcGenericMT2" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODBase" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthContainers" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthContainersInterfaces" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/CxxUtils" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthLinksSA" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/xAODRootAccessInterfaces" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODMissingET" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODCore" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODJet" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODBTagging" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTracking" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/DetectorDescription/GeoPrimitives" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/EventPrimitives" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODMuon" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODCaloEvent" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Calorimeter/CaloGeoHelpers" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODPrimitives" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/MuonSpectrometer/MuonIdHelpers" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTrigger" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODPFlow" -I"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CalcGenericMT2" -I"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CalcGenericMT2" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODBase" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthContainers" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthContainersInterfaces" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/CxxUtils" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthLinksSA" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/xAODRootAccessInterfaces" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODMissingET" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODCore" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODJet" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODBTagging" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTracking" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/DetectorDescription/GeoPrimitives" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/EventPrimitives" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODMuon" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODCaloEvent" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Calorimeter/CaloGeoHelpers" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODPrimitives" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/MuonSpectrometer/MuonIdHelpers" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTrigger" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODPFlow" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/eigen3" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include" "CalcGenericMT2/MT2_ATLAS.h" "CalcGenericMT2/MT2_ROOT.h" "Root/LinkDef.h"
/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake -E copy CalcGenericMT2LibCintDict.cxx  "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CalcGenericMT2/CMakeFiles/CalcGenericMT2LibCintDict.cxx"
/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake -E copy CalcGenericMT2Lib.dsomap "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CalcGenericMT2/CMakeFiles/CalcGenericMT2Lib.dsomap"
/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake -E copy libCalcGenericMT2Lib_rdict.pcm "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/lib/libCalcGenericMT2Lib_rdict.pcm"
