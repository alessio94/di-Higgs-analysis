#########################################################################################################
# Package: CAFCore ######################################################################################
cmake_minimum_required( VERSION 3.0.0 )

# Declare the package name:
project( CAFCore )

set(BINARY_TAG "${ATLAS_PLATFORM}")

# check if this is the top-level CMakeLists.txt
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(HAS_PARENT 0)
else()
  set(HAS_PARENT 1)
endif()

# add a helper function to manage lists
function (listToString result delim)
  list(GET ARGV 2 temp)
	list(LENGTH ARGV M)
  math(EXPR N "${M}-1")
	if(2 GREATER ${N})
		foreach(IDX RANGE 3 ${N})
			list(GET ARGV ${IDX} STR)
			set(temp "${temp}${delim}${STR}")
		endforeach()
	endif()
  set(${result} "${temp}" PARENT_SCOPE)
endfunction(listToString)

if(UNIX AND NOT APPLE)
	set(LINUX TRUE)
endif()

if( (NOT DEFINED NOCXX) OR (NOT ${NOCXX}) )
  find_package(LibXml2 REQUIRED)
endif()
# print what the LIBXML2_LIBRARIES variable looks before after string parsing
#message(STATUS "LIBXML2_LIBRARIES before string parsing: ${LIBXML2_LIBRARIES}")
# the string parsing here is needed for now as a bit of a hack since
# building on top of an AnalysisBase release, as of 21.2.9,
# causes the LIBXML2_LIBRARIES variable to be set using generator expressions
# e.g.
# $<BUILD_INTERFACE:/usr/lib64/libxml2.so>;$<INSTALL_INTERFACE:/usr/lib64/libxml2.so>
# and this string gets written to the cafsetup script below, before the generator expression is resolved
list(GET LIBXML2_LIBRARIES 0 LIBXML2_LIBRARIES)
string(REPLACE "$<BUILD_INTERFACE:" "" LIBXML2_LIBRARIES "${LIBXML2_LIBRARIES}")
string(REPLACE ">" "" LIBXML2_LIBRARIES "${LIBXML2_LIBRARIES}")
# print what the LIBXML2_LIBRARIES variable looks like after string parsing
#message(STATUS "LIBXML2_LIBRARIES after string parsing: ${LIBXML2_LIBRARIES}")

# write a setup script
set(CAFPLUGINSETUP ${CMAKE_BINARY_DIR}/CAFCorePluginSetup)
file(MAKE_DIRECTORY ${CAFPLUGINSETUP})
set(CAFSETUP ${CMAKE_CURRENT_BINARY_DIR}/cafsetup.sh)
file(WRITE ${CAFSETUP} "#!/bin/bash\n")
file(APPEND ${CAFSETUP} "# this is an auto-generated setup script\n" )
file(APPEND ${CAFSETUP} "export CAFANALYSISSETUP=${CMAKE_CURRENT_BINARY_DIR}/cafsetup.sh\n" )
file(APPEND ${CAFSETUP} "export CAFCOREDIR=${CMAKE_CURRENT_SOURCE_DIR}\n")
file(APPEND ${CAFSETUP} "export CAFBUILDDIR=${CMAKE_BINARY_DIR}\n")
file(APPEND ${CAFSETUP} "export TQPATH=${PROJECT_SOURCE_DIR}/QFramework\n")
file(APPEND ${CAFSETUP} "export PATH=\${PATH}:\${TQPATH}/share:${PROJECT_SOURCE_DIR}/CommonAnalysisHelpers/share:${PROJECT_SOURCE_DIR}/SFramework/share\n\n")
file(APPEND ${CAFSETUP} "if [ \"$(find ${CAFPLUGINSETUP} -name '*.sh' -print -quit 2>/dev/null)\" ]; then\n")
file(APPEND ${CAFSETUP} "for setupscript in ${CAFPLUGINSETUP}/*.sh; do source \${setupscript}; done\n")
file(APPEND ${CAFSETUP} "fi\n")

if(LINUX)
  set(NPROC "nproc")
elseif(APPLE)
  set(NPROC "sysctl -n hw.ncpu")
else()
  set(NPROC "")
endif()

if(NPROC STREQUAL "")
  file(APPEND ${CAFSETUP} "function cafbuild(){\n")
  file(APPEND ${CAFSETUP} "    cd ${CMAKE_BINARY_DIR} && make\n")
  file(APPEND ${CAFSETUP} "    local retVal=$?\n")
  file(APPEND ${CAFSETUP} "    cd -\n")
  file(APPEND ${CAFSETUP} "    return $retVal\n")
  file(APPEND ${CAFSETUP} "}\n")
else()
  file(APPEND ${CAFSETUP} "function cafbuild(){\n")
  file(APPEND ${CAFSETUP} "    local nProcessors=$(($(${NPROC})-1))\n")
  file(APPEND ${CAFSETUP} "    if [[ \"$nProcessors\" -lt 1 ]]; then\n")
  file(APPEND ${CAFSETUP} "        nProcessors=\"1\"\n")
  file(APPEND ${CAFSETUP} "    fi\n")
  file(APPEND ${CAFSETUP} "    cd ${CMAKE_BINARY_DIR} && make -j \${CAF_BUILD_CORES:-$nProcessors}\n")
  file(APPEND ${CAFSETUP} "    local retVal=$?\n")
  file(APPEND ${CAFSETUP} "    cd -\n")
  file(APPEND ${CAFSETUP} "    return $retVal\n")
  file(APPEND ${CAFSETUP} "}\n")
endif()

file(APPEND ${CAFSETUP} "function cafcompile(){\n")
file(APPEND ${CAFSETUP} "    cd ${CMAKE_BINARY_DIR} && cmake ${CMAKE_SOURCE_DIR}\n")
file(APPEND ${CAFSETUP} "    local retVal=$?\n")
file(APPEND ${CAFSETUP} "    cd -\n")
file(APPEND ${CAFSETUP} "    if [[ retVal -gt 0 ]]; then\n")
file(APPEND ${CAFSETUP} "        return $retVal\n")
file(APPEND ${CAFSETUP} "    fi\n")
file(APPEND ${CAFSETUP} "    cafbuild\n")
file(APPEND ${CAFSETUP} "    retVal=$?\n")
file(APPEND ${CAFSETUP} "    return $retVal\n")
file(APPEND ${CAFSETUP} "}\n\n")

file(APPEND ${CAFSETUP} "alias cafdiagnostics='bash ${PROJECT_SOURCE_DIR}/cafdiagnostics.sh 2>&1 | tee cafdiagnostics.txt; echo Wrote diagnostics output to $PWD/cafdiagnostics.txt'\n")
file(APPEND ${CAFSETUP} "alias tqpy='python -i $TQPATH/share/tqpy'\n")
#file(APPEND ${CAFSETUP} "alias cafsubmit='python ${PROJECT_SOURCE_DIR}/CommonAnalysisHelpers/share/cafsubmit'\n")

# collect all known packages
get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
  if (_variableName MATCHES "(.*)_SOURCE_DIR")
    if (NOT (${CMAKE_MATCH_1} STREQUAL "CMAKE_CURRENT" OR ${CMAKE_MATCH_1} STREQUAL "CMAKE" OR ${CMAKE_MATCH_1} STREQUAL "PROJECT"))
      list(APPEND PACKAGES_LIST "${CMAKE_MATCH_1}")
    endif()
  endif()
endforeach()

string(REPLACE ";" " " PACKAGES_STRTMP "${PACKAGES_LIST}")
# export to parent_scope, otherwise it's not picked up inside QFramework's CMakeLists

IF(${HAS_PARENT})
  set(PACKAGES_STR "${PACKAGES_STRTMP}" PARENT_SCOPE)
ELSE()
  set(PACKAGES_STR "${PACKAGES_STRTMP}")
ENDIF()

find_package(RooFitUtils QUIET)
list (FIND PACKAGES_LIST "RooFitUtils" _index)
if (${_index} GREATER -1)
  set(RooFitUtils_FOUND 1 PARENT_SCOPE)
endif()

find_package(CommonSmoothingTool QUIET)
list (FIND PACKAGES_LIST "CommonSmoothingTool" _index)
if (${_index} GREATER -1)
  IF(${HAS_PARENT})
    set(CommonSmoothingTool_FOUND 1 PARENT_SCOPE)
  ELSE()
    set(CommonSmoothingTool_FOUND 1)
  ENDIF()
endif()

# we try to discover if we are compiling inside an ASG environment or not
find_package( AnalysisBase QUIET)
find_package( AthAnalysis QUIET)
find_package( Athena QUIET )

IF(${AnalysisBase_FOUND} OR ${AthAnalysis_FOUND})
  set(AnalysisReleaseType AnalysisBase)
  if(${AthAnalysis_FOUND})
    set(AnalysisReleaseType AthAnalysis)
  elseif(${Athena_FOUND})
    set(AnalysisReleaseType Athena)
  ENDIF()

  set(AnalysisReleaseVersion ${${AnalysisReleaseType}_VERSION})

  # this is ASG compilation, we don't need to do anything

  atlas_platform_id( BINARY_TAG )

  message( "compiling CAFCore in AnalysisBase mode for " ${BINARY_TAG})

  # Set up the usage of CTest:
  IF(NOT ${HAS_PARENT})
    atlas_ctest_setup() # Set up the project:
    atlas_project( CAFCore 1.0.0
      USE ${AnalysisReleaseType} ${AnalysisReleaseVersion} )

    # Generate an environment setup script:
    lcg_generate_env( SH_FILE ${CMAKE_BINARY_DIR}/${BINARY_TAG}/env_setup.sh )
    install( FILES ${CMAKE_BINARY_DIR}/${BINARY_TAG}/env_setup.sh DESTINATION . )

    # Set up the usage of CPack:
    set(CMAKE_INSTALL_PREFIX /InstallArea/${BINARY_TAG})
    atlas_cpack_setup()
  ENDIF()

  file(APPEND ${CAFSETUP} "export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:${CMAKE_BINARY_DIR}/${BINARY_TAG}/lib\n")
  file(APPEND ${CAFSETUP} "export DYLD_LIBRARY_PATH=\${DYLD_LIBRARY_PATH}:${CMAKE_BINARY_DIR}/${BINARY_TAG}/lib\n")
  file(APPEND ${CAFSETUP} "export CAF_LIBRARIES=\${CAF_LIBRARIES}:${CMAKE_BINARY_DIR}/${BINARY_TAG}/lib\n")
  file(APPEND ${CAFSETUP} "export ROOT_INCLUDE_PATH=\${ROOT_INCLUDE_PATH}:${CMAKE_BINARY_DIR}/${BINARY_TAG}/include\n")



  # register this as a package to ASG
  atlas_subdir( CAFCore )

  file(APPEND ${CAFSETUP} "export PYTHONPATH=\${PYTHONPATH}:${CMAKE_CURRENT_BINARY_DIR}:${CMAKE_BINARY_DIR}/${BINARY_TAG}/python:${CMAKE_BINARY_DIR}/${BINARY_TAG}/lib\n")
  file(APPEND ${CAFSETUP} "export LIBXMLPATH=${LIBXML2_LIBRARIES}\n")

  # Add all targets to the build-tree export set
  if( (NOT ${HAS_PARENT}) AND ((NOT DEFINED NOCXX) OR (NOT ${NOCXX})) )
    set(CONF_LIBRARY_DIRS "${PROJECT_BINARY_DIR}/${BINARY_TAG}/lib")
  endif()

  # Export the package for use from the build-tree
  # (this registers the build-tree with a global CMake-registry)
  export(PACKAGE CAFCore)
  configure_file(CAFCoreConfig.cmake.in
    "${PROJECT_BINARY_DIR}/CAFCoreConfig.cmake" @ONLY)

  # Install the CAFCoreConfig.cmake and CAFCoreConfigVersion.cmake
  install(FILES
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CAFCoreConfig.cmake"
    "${PROJECT_BINARY_DIR}/CAFCoreConfigVersion.cmake"
    DESTINATION "${PROJECT_SOURCE_DIR}" COMPONENT dev)

  if( (NOT ${HAS_PARENT}) AND (NOT DEFINED NOCXX) OR (NOT ${NOCXX}) )
    if( ${RooFitUtils_FOUND} AND ${CommonSmoothingTool_FOUND})
      export(TARGETS QFramework QFrameworkPkg SFramework SFrameworkPkg CAFxAODUtils CAFxAODUtilsPkg RooFitUtils RooFitUtilsPkg CommonSmoothingTool FILE "${PROJECT_BINARY_DIR}/CAFCoreTargets.cmake")
      set(CONF_LIBRARIES    QFramework SFramework CAFxAODUtils RooFitUtils CommonSmoothingTool)
      set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/QFramework" "${PROJECT_SOURCE_DIR}/SFramework" "${PROJECT_SOURCE_DIR}/CAFxAODUtils" "${PROJECT_SOURCE_DIR}/RooFitUtils" "${PROJECT_SOURCE_DIR}/CommonSystSmoothingTool")
    elseif( ${RooFitUtils_FOUND} )
      export(TARGETS QFramework QFrameworkPkg SFramework SFrameworkPkg CAFxAODUtils CAFxAODUtilsPkg RooFitUtils RooFitUtilsPkg FILE "${PROJECT_BINARY_DIR}/CAFCoreTargets.cmake")
      set(CONF_LIBRARIES    QFramework SFramework CAFxAODUtils RooFitUtils)
      set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/QFramework" "${PROJECT_SOURCE_DIR}/SFramework" "${PROJECT_SOURCE_DIR}/CAFxAODUtils" "${PROJECT_SOURCE_DIR}/RooFitUtils")
    elseif( ${CommonSmoothingTool_FOUND} )      
      export(TARGETS QFramework QFrameworkPkg SFramework SFrameworkPkg CAFxAODUtils CAFxAODUtilsPkg CommonSmoothingTool FILE "${PROJECT_BINARY_DIR}/CAFCoreTargets.cmake")
      set(CONF_LIBRARIES    QFramework SFramework CAFxAODUtils CommonSmoothingTool)
      set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/QFramework" "${PROJECT_SOURCE_DIR}/SFramework" "${PROJECT_SOURCE_DIR}/CAFxAODUtils" "${PROJECT_SOURCE_DIR}/CommonSystSmoothingTool")
    else()
      export(TARGETS QFramework QFrameworkPkg SFramework SFrameworkPkg CAFxAODUtils CAFxAODUtilsPkg FILE "${PROJECT_BINARY_DIR}/CAFCoreTargets.cmake")
      set(CONF_LIBRARIES    QFramework SFramework CAFxAODUtils)
      set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/QFramework" "${PROJECT_SOURCE_DIR}/SFramework" "${PROJECT_SOURCE_DIR}/CAFxAODUtils")
    endif()
  endif()
  
else()
  # this is standalone compilation, we need to stitch together the project
  message( "compiling CAFCore in standalone mode" )
  set(WITHOUT_ASG ON )

  file(APPEND ${CAFSETUP} "export PYTHONPATH=\${PYTHONPATH}:${CMAKE_CURRENT_BINARY_DIR}\n")
  file(APPEND ${CAFSETUP} "export LIBXMLPATH=${LIBXML2_LIBRARIES}\n")

  # set general CMake settings
  set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
  set(CMAKE_DISABLE_SOURCE_CHANGES  OFF)
  set(CMAKE_VERBOSE_MAKEFILE OFF)
  set(CMAKE_COLOR_MAKEFILE   ON)

  include(CheckCXXCompilerFlag)

  # Check for standard to use
  if(DEFINED CXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${CXX}")
  else()
    check_cxx_compiler_flag(-std=c++17 HAVE_FLAG_STD_CXX17)
    if(HAVE_FLAG_STD_CXX17)
      set(CMAKE_CXX_STANDARD 17)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    else()
      check_cxx_compiler_flag(-std=c++14 HAVE_FLAG_STD_CXX14)
      if(HAVE_FLAG_STD_CXX14)
	set(CMAKE_CXX_STANDARD 14)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
      else()
	set(CMAKE_CXX_STANDARD 11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
      endif()
    endif()
  endif()
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  # add some compiler flags
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

  # require ROOT
  if( (NOT DEFINED NOCXX) OR (NOT ${NOCXX}) )
    find_package( ROOT REQUIRED )
    include(${ROOT_USE_FILE})
    # Include boost headers
    find_package( Boost REQUIRED)
    include_directories(${Boost_INCLUDE_DIR})
  endif()

  # enabling python test cases
  find_package(PythonInterp REQUIRED)
  include(CTest)
  enable_testing()

  # When done tweaking common stuff, configure the components (subprojects).
  # NOTE: The order matters! The most independent ones should go first.
  if( (NOT DEFINED NOCXX) OR (NOT ${NOCXX}) )
    message("adding QFramework")
    add_subdirectory(QFramework)
    if(EXISTS ${PROJECT_SOURCE_DIR}/RooFitUtils)
      message("adding RooFitUtils")
      add_subdirectory(RooFitUtils)
    endif()
    if(EXISTS ${PROJECT_SOURCE_DIR}/SFramework)
      message("adding SFramework")
      add_subdirectory(SFramework)
    endif()
  endif()
  add_subdirectory(CommonAnalysisHelpers)

  # Add all targets to the build-tree export set
  if( (NOT DEFINED NOCXX) OR (NOT ${NOCXX}) )
    set(CONF_LIBRARY_DIRS "${PROJECT_BINARY_DIR}")
  endif()

  # Export the package for use from the build-tree
  # (this registers the build-tree with a global CMake-registry)
  export(PACKAGE CAFCore)
  configure_file(CAFCoreConfig.cmake.in
    "${PROJECT_BINARY_DIR}/CAFCoreConfig.cmake" @ONLY)

  # Install the CAFCoreConfig.cmake and CAFCoreConfigVersion.cmake
  install(FILES
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CAFCoreConfig.cmake"
    "${PROJECT_BINARY_DIR}/CAFCoreConfigVersion.cmake"
    DESTINATION "${PROJECT_SOURCE_DIR}" COMPONENT dev)

  if( (NOT DEFINED NOCXX) OR (NOT ${NOCXX}) )
    if( ${RooFitUtils_FOUND} AND ${CommonSmoothingTool_FOUND})
      export(TARGETS QFramework SFramework RooFitUtils CommonSmoothingTool FILE "${PROJECT_BINARY_DIR}/CAFCoreTargets.cmake")
      set(CONF_LIBRARIES QFramework SFramework CAFxAODUtils RooFitUtils CommonSmoothingTool)
      set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/QFramework" "${PROJECT_SOURCE_DIR}/SFramework" "${PROJECT_SOURCE_DIR}/CAFxAODUtils" "${PROJECT_SOURCE_DIR}/RooFitUtils" "${PROJECT_SOURCE_DIR}/CommonSystSmoothingTool")
    elseif( ${RooFitUtils_FOUND} )
      export(TARGETS QFramework SFramework RooFitUtils FILE "${PROJECT_BINARY_DIR}/CAFCoreTargets.cmake")
      set(CONF_LIBRARIES    QFramework SFramework CAFxAODUtils RooFitUtils)
      set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/QFramework" "${PROJECT_SOURCE_DIR}/SFramework" "${PROJECT_SOURCE_DIR}/CAFxAODUtils" "${PROJECT_SOURCE_DIR}/RooFitUtils")
    elseif( ${CommonSmoothingTool_FOUND} )      
      export(TARGETS QFramework SFramework CommonSmoothingTool FILE "${PROJECT_BINARY_DIR}/CAFCoreTargets.cmake")
      set(CONF_LIBRARIES    QFramework SFramework CAFxAODUtils CommonSmoothingTool)
      set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/QFramework" "${PROJECT_SOURCE_DIR}/SFramework" "${PROJECT_SOURCE_DIR}/CAFxAODUtils" "${PROJECT_SOURCE_DIR}/CommonSystSmoothingTool")
    else()
      export(TARGETS QFramework SFramework FILE "${PROJECT_BINARY_DIR}/CAFCoreTargets.cmake")
      set(CONF_LIBRARIES    QFramework SFramework CAFxAODUtils)
      set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/QFramework" "${PROJECT_SOURCE_DIR}/SFramework" "${PROJECT_SOURCE_DIR}/CAFxAODUtils")
    endif()
  endif()
endif()


  
