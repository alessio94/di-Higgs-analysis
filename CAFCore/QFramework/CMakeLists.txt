#########################################################################################################
# Package: QFramework ###################################################################################

project( QFramework )
set(TQPATH ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
set(TQPATH ${CMAKE_CURRENT_SOURCE_DIR})

IF(NOT WITHOUT_ASG)
  # register this as a package to ASG
  atlas_subdir( QFramework )
ENDIF()

find_package(CommonSmoothingTool QUIET)

# look for and add the common smoothing tools
if(${CommonSmoothingTool_FOUND})
  include_directories ("${CommonSmoothingTool_CMAKE_DIR}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${CommonSmoothingTool_INCLUDE_DIRS} -DHAS_CommonSmoothingTool")
  set(OPTIONAL_COMMONSMOOTHINGTOOL "CommonSmoothingTool")
  set(OPTIONAL_COMMONSMOOTHINGTOOLLIB "CommonSystSmoothingToolLib")  
  message("using CommonSmoothingTool component")
else()
  set(OPTIONAL_COMMONSMOOTHINGTOOL "")
  set(OPTIONAL_COMMONSMOOTHINGTOOLLIB "")
  message("CommonSmoothingTool not found")  
endif()

# include libxml
find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})
set(LIBS ${LIBS} ${LIBXML2_LIBRARIES})

# print what the LIBXML2_LIBRARIES variable looks like before string parsing
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
# include boost
find_package(Boost)
if(${Boost_FOUND})
  include_directories(${BOOST_INCLUDE_DIR})
  set(LIBS ${LIBS} ${BOOST_LIBRARIES})
else()
  if(DEFINED ENV{ALRB_BOOST_ROOT})
    message("manually activating boost from ALRB")
    include_directories($ENV{ALRB_BOOST_ROOT}/include)
  else()
    message( SEND_ERROR "Boost is required to compile QFramework!")  
  endif()
endif()


# include ROOT
include_directories ("${PROJECT_SOURCE_DIR}")
find_package( ROOT COMPONENTS Core Tree MathCore Hist RIO HistPainter Matrix Gpad Graf TreePlayer TMVA )

# set some variables for easier handling
set(QFrameworkLinkDef ${CMAKE_CURRENT_BINARY_DIR}/LinkDef.h)
if(NOT WITHOUT_ASG)
  # With ASG, use x86_64 location for python modules
  set(QFrameworkPythonInit ${CMAKE_PYTHON_OUTPUT_DIRECTORY}/QFramework/__init__.py)
  # Remove old files, to force switch to new directory
  file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/__init__.py)
  file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/__init__.pyc)
ELSE()
  # Use legacy path
  set(QFrameworkPythonInit ${CMAKE_CURRENT_BINARY_DIR}/__init__.py)
ENDIF()

file(GLOB QFrameworkPython "python/*.py") #note: this does not include __init__.py since it's not generated yet!
file(GLOB QFrameworkSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Root/[A-Za-z]*.cxx")
file(GLOB QFrameworkHeaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "QFramework/[A-Za-z]*.h")
file(GLOB QFrameworkPythonTests "test/[A-Za-z]*.py")

#########################################################################################################
### ATTENTION: The auto-generated files are currently created at configuration time, not at build time! #

# generate the LinkDef.h and __init__.py
execute_process(
  COMMAND python ${TQPATH}/share/generateBindings.py --set-working-directory ${CMAKE_CURRENT_SOURCE_DIR} --pkgname QFramework --verbose --python ${QFrameworkPythonInit} --python-head  share/QFramework_template.py --linkdef ${QFrameworkLinkDef} --headers ${QFrameworkHeaders}
)

# generate the definitions.h
execute_process(
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/share/generateLocals.py --verbose  --output ${CMAKE_CURRENT_BINARY_DIR}/definitions.h --set-working-directory ${CMAKE_CURRENT_SOURCE_DIR} --root --git --svn --gcc --set-named-path TQPATH ${CMAKE_CURRENT_SOURCE_DIR} --flag-header HAS_ROOLAGRANGIANMORPHING RooLagrangianMorphing.h --find-binary EXIFTOOLPATH exiftool --set-named-path LIBXMLPATH ${LIBXML2_LIBRARIES} --packages ${PACKAGES_STR}
)
# generate the locals.h
execute_process(
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/share/generateLocals.py --verbose  --output ${CMAKE_CURRENT_BINARY_DIR}/locals.h --set-working-directory ${CMAKE_CURRENT_SOURCE_DIR}
)

#########################################################################################################

if(NOT WITHOUT_ASG)
       
  # this section reflects the standard ASG way of configuring CMake
  # it is executed when compiling within an ASG environment
  find_package( GTest )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAS_XAOD")
  atlas_add_root_dictionary( QFramework QFrameworkCintDict
    ROOT_HEADERS ${QFrameworkHeaders} ${QFrameworkLinkDef}
    EXTERNAL_PACKAGES ROOT )
  atlas_add_library( QFramework
    ${QFrameworkHeaders} ${QFrameworkSources} ${QFrameworkCintDict}
    PUBLIC_HEADERS QFramework
    INCLUDE_DIRS $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> ${ROOT_INCLUDE_DIRS} 
    LINK_LIBRARIES xAODCore xAODRootAccess xAODTruth xAODBase xAODCutFlow ${ROOT_LIBRARIES} ${OPTIONAL_COMMONSMOOTHINGTOOLLIB}
    PRIVATE_LINK_LIBRARIES ${LIBXML2_LIBRARIES}
    )
  atlas_install_python_modules( ${QFrameworkPython} )

  # register the test cases
  find_package(PythonInterp REQUIRED)
  foreach(TestScript ${QFrameworkPythonTests})
    get_filename_component(TestName ${TestScript} NAME_WE)
    atlas_add_test( ${TestName} SCRIPT ${TestScript} )
  endforeach()
ELSE()
  # this section is a standalone compilation segment
  # it is executed when compiling outside an ASG environment

  foreach(module ${QFrameworkPython})
    execute_process(
      COMMAND ln -sf ${module} ${CMAKE_CURRENT_BINARY_DIR}/
      )
  endforeach()

  file(APPEND ${CAFSETUP} "export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:${CMAKE_CURRENT_BINARY_DIR}/\n")
  file(APPEND ${CAFSETUP} "export DYLD_LIBRARY_PATH=\${DYLD_LIBRARY_PATH}:${CMAKE_CURRENT_BINARY_DIR}/\n")
  file(APPEND ${CAFSETUP} "export CAF_LIBRARIES=\${CAF_LIBRARIES}:${CMAKE_CURRENT_BINARY_DIR}/\n")

  # register all the files and directories
  include_directories ("${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" "${ROOT_INCLUDE_DIRS}")

  # generate the dictionary source code
  ROOT_GENERATE_DICTIONARY(G__QFramework ${QFrameworkHeaders} LINKDEF ${QFrameworkLinkDef})

  # register the shared object to include both sources and dictionaries
  add_library( QFramework SHARED ${QFrameworkSources} G__QFramework.cxx)

  # link everything together at the end
  target_link_libraries( QFramework  ${OPTIONAL_COMMONSMOOTHINGTOOL} ${LIBXML2_LIBRARIES} ${ROOT_LIBRARIES})

  # register the test cases
  foreach(TestScript ${QFrameworkPythonTests})
    get_filename_component(TestName ${TestScript} NAME_WE)
    add_test( NAME ${TestName} COMMAND ${TestScript})
  endforeach()
ENDIF()
