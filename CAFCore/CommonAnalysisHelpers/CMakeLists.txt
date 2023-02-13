#########################################################################################################
# Package: CommonAnalysisHelpers#########################################################################

project( CommonAnalysisHelpers )

file(GLOB CommonAnalysisHelpersPythonTests test/test_[A-Za-z]*.py)

IF(NOT WITHOUT_ASG)
  # register this as a package to ASG
  atlas_subdir( CommonAnalysisHelpers )
ENDIF()

if(NOT WITHOUT_ASG)
  # this section reflects the standard ASG way of configuring CMake
  # it is executed when compiling within an ASG environment
  find_package( GTest )
  atlas_install_python_modules( python/* )
  # register the test cases
  find_package(PythonInterp REQUIRED)
  foreach(TestScript ${CommonAnalysisHelpersPythonTests})
    get_filename_component(TestName ${TestScript} NAME_WE)
    atlas_add_test( ${TestName} SCRIPT ${TestScript} )
    set_tests_properties(CommonAnalysisHelpers_${TestName}_ctest PROPERTIES TIMEOUT 600) 
  endforeach()
ELSE()
  # link all the python files
  file(GLOB pyfiles "python/*")
  foreach(pyfile ${pyfiles})
    execute_process(
      COMMAND ln -sf ${pyfile} ${CMAKE_CURRENT_BINARY_DIR}
      )
  endforeach()
  # register the test cases
  foreach(TestScript ${CommonAnalysisHelpersPythonTests})
    get_filename_component(TestName ${TestScript} NAME_WE)
    add_test( NAME ${TestName} COMMAND ${TestScript})
    set_tests_properties(${TestName} PROPERTIES TIMEOUT 600) 
  endforeach()
ENDIF()
