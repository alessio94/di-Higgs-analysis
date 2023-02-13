set -e
tmpdir=`mktemp -d makeQFrameworkCintDict.XXXXXX`
cd ${tmpdir}
/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/bin/rootcling -f QFrameworkCintDict.cxx -s libQFramework.so -rml libQFramework.so -rmf QFramework.dsomap -DHAVE_PRETTY_FUNCTION -DHAVE_64_BITS -D__IDENTIFIER_64BIT__ -DATLAS -DROOTCORE -DXAOD_STANDALONE -DXAOD_ANALYSIS -DROOTCORE_RELEASE_SERIES=25 -DPACKAGE_VERSION=\"QFramework-00-00-00\" -DPACKAGE_VERSION_UQ=QFramework-00-00-00 -I"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework" -I"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework" -I"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework" -I"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework" -I"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/QFramework" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/libxml2" -I"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODCore" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthContainers" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthContainersInterfaces" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/CxxUtils" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/AthLinksSA" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/xAODRootAccessInterfaces" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Control/xAODRootAccess" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODEventFormat" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODTruth" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODBase" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/../../../../AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/src/Event/xAOD/xAODCutFlow" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/RootCore/include" -I"/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBaseExternals/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/include/libxml2" -I"/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/QFramework" "QFramework/ASG.h" "QFramework/TQABCDCalculator.h" "QFramework/TQAlgorithm.h" "QFramework/TQAnalysisAlgorithm.h" "QFramework/TQAnalysisJob.h" "QFramework/TQAnalysisSampleVisitor.h" "QFramework/TQAnalysisSampleVisitorBase.h" "QFramework/TQCompPlotter.h" "QFramework/TQCompiledCut.h" "QFramework/TQConfigReader.h" "QFramework/TQConstObservable.h" "QFramework/TQConstTreeObservable.h" "QFramework/TQCounter.h" "QFramework/TQCut.h" "QFramework/TQCutFactory.h" "QFramework/TQCutflowAnalysisJob.h" "QFramework/TQCutflowPlotter.h" "QFramework/TQCutflowPrinter.h" "QFramework/TQDefaultPlotter.h" "QFramework/TQEventIndexObservable.h" "QFramework/TQEventlistAnalysisJob.h" "QFramework/TQEventlistPrinter.h" "QFramework/TQFilterObservable.h" "QFramework/TQFlags.h" "QFramework/TQFolder.h" "QFramework/TQGraphMakerAnalysisJob.h" "QFramework/TQGridScanBound.h" "QFramework/TQGridScanObservable.h" "QFramework/TQGridScanPoint.h" "QFramework/TQGridScanResults.h" "QFramework/TQGridScanStyle.h" "QFramework/TQGridScanner.h" "QFramework/TQHWWPlotter.h" "QFramework/TQHistComparer.h" "QFramework/TQHistoMakerAnalysisJob.h" "QFramework/TQHistogramObservable.h" "QFramework/TQHistogramUtils.h" "QFramework/TQImportLink.h" "QFramework/TQIterator.h" "QFramework/TQIteratorT.h" "QFramework/TQLibrary.h" "QFramework/TQLink.h" "QFramework/TQListUtils.h" "QFramework/TQMVA.h" "QFramework/TQMVAObservable.h" "QFramework/TQMessageStream.h" "QFramework/TQMultiChannelAnalysisSampleVisitor.h" "QFramework/TQMultiObservable.h" "QFramework/TQNFBase.h" "QFramework/TQNFCalculator.h" "QFramework/TQNFChainloader.h" "QFramework/TQNFCustomCalculator.h" "QFramework/TQNFManualSetter.h" "QFramework/TQNFTop0jetEstimator.h" "QFramework/TQNFTop1jetEstimator.h" "QFramework/TQNFUncertaintyScaler.h" "QFramework/TQNTupleDumperAnalysisJob.h" "QFramework/TQNamedTaggable.h" "QFramework/TQObservable.h" "QFramework/TQPCA.h" "QFramework/TQPCAAnalysisJob.h" "QFramework/TQPathManager.h" "QFramework/TQPlotter.h" "QFramework/TQPresenter.h" "QFramework/TQROOTPlotter.h" "QFramework/TQSample.h" "QFramework/TQSampleDataReader.h" "QFramework/TQSampleFolder.h" "QFramework/TQSampleGroupingVisitor.h" "QFramework/TQSampleInitializer.h" "QFramework/TQSampleInitializerBase.h" "QFramework/TQSampleNormalizationObservable.h" "QFramework/TQSamplePurger.h" "QFramework/TQSampleRevisitor.h" "QFramework/TQSampleVisitor.h" "QFramework/TQSignificanceEvaluator.h" "QFramework/TQStringUtils.h" "QFramework/TQSystematics.h" "QFramework/TQSystematicsHandler.h" "QFramework/TQSystematicsManager.h" "QFramework/TQTHnBaseMakerAnalysisJob.h" "QFramework/TQTHnBaseUtils.h" "QFramework/TQTable.h" "QFramework/TQTaggable.h" "QFramework/TQTikZPlotter.h" "QFramework/TQToken.h" "QFramework/TQTreeFormulaObservable.h" "QFramework/TQTreeObservable.h" "QFramework/TQUniqueCut.h" "QFramework/TQUtils.h" "QFramework/TQValue.h" "QFramework/TQVectorAuxObservable.h" "QFramework/TQXSecParser.h" "../../build/CAFCore/QFramework/LinkDef.h"
/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake -E copy QFrameworkCintDict.cxx  "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/QFramework/CMakeFiles/QFrameworkCintDict.cxx"
/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake -E copy QFramework.dsomap "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/QFramework/CMakeFiles/QFramework.dsomap"
/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake -E copy libQFramework_rdict.pcm "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/lib/libQFramework_rdict.pcm"