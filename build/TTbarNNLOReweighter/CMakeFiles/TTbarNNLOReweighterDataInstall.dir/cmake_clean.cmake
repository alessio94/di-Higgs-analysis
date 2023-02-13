file(REMOVE_RECURSE
  "../x86_64-centos7-gcc8-opt/data/TTbarNNLOReweighter/TTbar_NNLO_QCD_NLO_EW_weights.root"
  "../x86_64-centos7-gcc8-opt/data/TTbarNNLOReweighter/TTbar_NNLO_QCD_NLO_EW_weights.root"
  "../x86_64-centos7-gcc8-opt/include/TTbarNNLOReweighter"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/TTbarNNLOReweighterDataInstall.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
