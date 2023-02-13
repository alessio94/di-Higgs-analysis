#########################################################################################################
# Package: SubmissionHelpers ############################################################################

cmake_minimum_required(VERSION 3.0.0)
project( SubmissionHelpers )

find_package(AnalysisBase QUIET)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(HAS_PARENT 0)
else()
  set(HAS_PARENT 1)
endif()

set(SETUP ${CMAKE_CURRENT_BINARY_DIR}/setup.sh)
file(WRITE ${SETUP} "#!/bin/bash\n")
file(APPEND ${SETUP} "# this is an auto-generated setup script\n" )
file(GLOB SubmissionHelpersPythonTests test/test_[A-Za-z]*.py)

if(${AnalysisBase_FOUND})
  # this section reflects the standard ASG way of configuring CMake
  # it is executed when compiling within an ASG environment
  atlas_platform_id( BINARY_TAG )
  IF(NOT ${HAS_PARENT})
    # Also change version in setup.py and module
    atlas_project( SubmissionHelpers 1.0.0 USE AnalysisBase ${AnalysisBase_VERSION} )
    
    # Generate an environment setup script:
    lcg_generate_env( SH_FILE ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh )
    install( FILES ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh DESTINATION . )
    
    # Set up the usage of CPack:
    set(CMAKE_INSTALL_PREFIX /InstallArea/x86_64-slc6-gcc49-opt)
    atlas_cpack_setup()
  ENDIF()
  atlas_subdir( SubmissionHelpers )
  IF(NOT ${HAS_PARENT})
    atlas_ctest_setup() # Set up the project:
  ENDIF()
  
  find_package( GTest )
  atlas_install_python_modules( python/* )
  find_package(PythonInterp REQUIRED)


  foreach(TestScript ${SubmissionHelpersPythonTests})
    get_filename_component(TestName ${TestScript} NAME_WE)
    atlas_add_test( ${TestName} SCRIPT ${TestScript} )
    set_tests_properties(SubmissionHelpers_${TestName}_ctest PROPERTIES TIMEOUT 600) 
  endforeach()
  
  file(APPEND ${SETUP} "export PYTHONPATH=\${PYTHONPATH}:${CMAKE_BINARY_DIR}/${BINARY_TAG}/python\n")
  file(APPEND ${SETUP} "export PATH=\${PATH}:${CMAKE_CURRENT_SOURCE_DIR}/share\n")
  
ELSE()
  # link all the python files
  file(GLOB pyfiles "python/*")

  set(PYTHONDIR ${CMAKE_CURRENT_BINARY_DIR}/SubmissionHelpers)    
  file(APPEND ${SETUP} "export PYTHONPATH=\${PYTHONPATH}:${CMAKE_CURRENT_BINARY_DIR}\n")
  file(APPEND ${SETUP} "export PATH=\${PATH}:${CMAKE_CURRENT_SOURCE_DIR}/share\n")

  execute_process(COMMAND mkdir -p ${PYTHONDIR})

  foreach(pyfile ${pyfiles})
    execute_process(
      COMMAND ln -sf ${pyfile} ${PYTHONDIR}
      )
  endforeach()

  # register the test cases
  enable_testing()
  
  foreach(TestScript ${SubmissionHelpersPythonTests})
    get_filename_component(TestName ${TestScript} NAME_WE)
    add_test( NAME ${TestName} COMMAND ${TestScript})
    set_tests_properties(${TestName} PROPERTIES TIMEOUT 600) 
  endforeach()
ENDIF()
