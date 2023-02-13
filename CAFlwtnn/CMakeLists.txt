#########################################################################################################
# Package: CAFLWTNN #####################################################################################
cmake_minimum_required( VERSION 2.8 )

# Declare the package name:
project( CAFlwtnn )

# check if this is the top-level CMakeLists.txt
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(HAS_PARENT 0)
else()
  set(HAS_PARENT 1)
endif()

include_directories ("${PROJECT_SOURCE_DIR}")
# we try to discover if we are compiling inside an ASG environment or not
find_package( AnalysisBase QUIET) 

# define dependencies on other packages
find_package( ROOT COMPONENTS Core)
find_package( lwtnn REQUIRED )
# set some variables for easier handling
file(GLOB CAFLWTNNSources Root/*.cxx)
file(GLOB CAFLWTNNHeaders CAFlwtnn/*.h)
set(CAFLWTNNLinkDef ${PROJECT_SOURCE_DIR}/Root/LinkDef.h)

#currently not used, as LinkDef.h is static for this package
#execute_process(
#    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/../CAFCore/QFramework/share/generateBindings.py --set-working-directory ${CMAKE_CURRENT_SOURCE_DIR} --pkgname CAFlwtnn --loadpackage --verbose --python ${PROJECT_SOURCE_DIR}/python/__init__.py --linkdef ${CAFLWTNNLinkDef} --headers ${CAFLWTNNHeaders}
#)

#create the plugin directory in case CAFCore's cmake file hasn't generated it yet. 
# *.sh files in this directory are automatically sourced by CAFCore's cafsetup.sh 
#(which in turn should be sourced by the analysis packages setup script)
file(MAKE_DIRECTORY file(MAKE_DIRECTORY ${CAFPLUGINSETUP}))
set(SETUP ${CMAKE_BINARY_DIR}/CAFCorePluginSetup/CAFlwtnnSetup.sh)
file(WRITE ${SETUP} "#!/bin/bash\n")
file(APPEND ${SETUP} "# this is an auto-generated setup script\n" )

IF(${AnalysisBase_FOUND})
  
  IF(NOT ${HAS_PARENT})
    atlas_ctest_setup() # Set up the project: 
    atlas_project( CAFlwtnn 1.0.0 
      USE AnalysisBase ${AnalysisBase_VERSION} ) 
    
    # Generate an environment setup script: 
    lcg_generate_env( SH_FILE ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh )
    install( FILES ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh DESTINATION . ) 

    # Set up the usage of CPack: 
    set(CMAKE_INSTALL_PREFIX /InstallArea/x86_64-slc6-gcc49-opt)
    atlas_cpack_setup()
  ENDIF()

  # this is ASG compilation, we don't need to do anything
  atlas_subdir( CAFlwtnn )
  
  atlas_depends_on_subdirs(PUBLIC QFramework)
  
  atlas_add_root_dictionary( CAFlwtnn CAFLWTNNCintDict
    ROOT_HEADERS ${CAFLWTNNHeaders} ${CAFLWTNNLinkDef}
    EXTERNAL_PACKAGES ROOT )

  atlas_add_library( CAFlwtnn
    ${CAFLWTNNHeaders} ${CAFLWTNNSources} ${CAFLWTNNCintDict}
    PUBLIC_HEADERS CAFlwtnn
    PRIVATE_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} ${QFramework_INCLUDE_DIRS} ${LWTNN_INCLUDE_DIRS}
    LINK_LIBRARIES QFramework 
    PRIVATE_LINK_LIBRARIES ${ROOT_LIBRARIES} ${LWTNN_LIBRARIES}
    )
  #possibly move this out of the ASG-compillation scope once stand-alone compillation is supported as it's general
  file(APPEND ${SETUP} "export CAF_PRELOAD_LIBRARIES=\${CAF_PRELOAD_LIBRARIES}:libCAFlwtnn\n")
  
  
else()
  # this is standalone compilation, we need to stitch together the project
  message( "compiling CAFLWTNN in standalone mode" )
  set(WITHOUT_ASG ON )

  find_package( CAFCore REQUIRED PATHS ${CMAKE_BINARY_DIR} NO_DEFAULT_PATH)

  # set general CMake settings
  set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
  set(CMAKE_DISABLE_SOURCE_CHANGES  OFF)  
  set(CMAKE_VERBOSE_MAKEFILE OFF)
  set(CMAKE_COLOR_MAKEFILE   ON)

  # require ROOT
  find_package( ROOT REQUIRED )
  include(${ROOT_USE_FILE})
 
  # enabling python test cases
  find_package(PythonInterp REQUIRED)
  include(CTest)
  enable_testing()

  include_directories (${CAFCore_INCLUDE_DIRS})

  # register the shared object to include both sources and dictionaries
  add_library( CAFlwtnn SHARED ${CAFLWTNNSources})

  file(APPEND ${SETUP} "export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:${CMAKE_CURRENT_BINARY_DIR}\n")
  file(APPEND ${SETUP} "export PATH=\${PATH}:${CMAKE_SOURCE_DIR}/scripts\n")
 
endif()
