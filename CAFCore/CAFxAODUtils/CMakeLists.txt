#########################################################################################################
# Package: CAFxAODUtils #################################################################################

cmake_minimum_required(VERSION 2.8)
project( CAFxAODUtils )

atlas_subdir( CAFxAODUtils )

find_package( ROOT COMPONENTS Core )

find_package( Eigen )

atlas_depends_on_subdirs(
   PUBLIC
   QFramework
   AtlasEigen
   Control/AthToolSupport/AsgTools
   Event/xAOD/xAODCore
   Event/xAOD/xAODParticleEvent
   Event/xAOD/xAODTruth
   Event/xAOD/xAODEventInfo
   Event/xAOD/AsgAnalysisInterfaces
)

# set some variables for easier handling
file(GLOB CAFxAODUtilsSources Root/*.cxx)
file(GLOB CAFxAODUtilsHeaders CAFxAODUtils/*.h)

# this section reflects the standard ASG way of configuring CMake
atlas_add_root_dictionary( CAFxAODUtils CAFxAODUtilsCintDict
  ROOT_HEADERS ${CAFxAODUtilsHeaders} Root/LinkDef.h
  EXTERNAL_PACKAGES ROOT
  )
atlas_add_library( CAFxAODUtils
  ${CAFxAODUtilsHeaders} ${CAFxAODUtilsSources} ${CAFxAODUtilsCintDict}
  PUBLIC_HEADERS CAFxAODUtils
  PRIVATE_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} ${EIGEN_INCLUDE_DIRS}
  LINK_LIBRARIES QFramework AsgTools xAODCore xAODTruth xAODParticleEvent
  PRIVATE_LINK_LIBRARIES ${ROOT_LIBRARIES}
  )

