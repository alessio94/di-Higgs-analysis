#########################################################################################################
# Package: CxAODUtils #################################################################################

cmake_minimum_required(VERSION 2.8)
project( CxAODUtils )

atlas_subdir( CxAODUtils )

find_package( ROOT COMPONENTS Core )

find_package( Eigen )

atlas_depends_on_subdirs(
   PUBLIC
   QFramework
   Control/AthToolSupport/AsgTools
   Event/xAOD/xAODCore
   Event/xAOD/xAODEventInfo
   PhysicsAnalysis/AnalysisCommon/AssociationUtils
   PhysicsAnalysis/AnalysisCommon/PATInterfaces
)

# set some variables for easier handling
file(GLOB CxAODUtilsSources Root/*.cxx)
file(GLOB CxAODUtilsHeaders CxAODUtils/*.h)

# this section reflects the standard ASG way of configuring CMake
atlas_add_root_dictionary( CxAODUtils CxAODUtilsCintDict
  ROOT_HEADERS ${CxAODUtilsHeaders} Root/LinkDef.h
  EXTERNAL_PACKAGES ROOT
  )
atlas_add_library( CxAODUtils
  ${CxAODUtilsHeaders} ${CxAODUtilsSources} ${CxAODUtilsCintDict}
  PUBLIC_HEADERS CxAODUtils
  PRIVATE_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} ${EIGEN_INCLUDE_DIRS}
  LINK_LIBRARIES QFramework AsgTools xAODCore AssociationUtilsLib PATInterfaces
  PRIVATE_LINK_LIBRARIES ${ROOT_LIBRARIES}
  )

