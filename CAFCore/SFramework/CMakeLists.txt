#########################################################################################################
# Package: SFramework ###################################################################################

project( SFramework )

# include libxml
find_package(LibXml2 REQUIRED)

IF(NOT WITHOUT_ASG)
  # register this as a package to ASG
  atlas_subdir( SFramework )
ENDIF()

if(${RooFitUtils_FOUND})
  include_directories ("${RooFitUtils_SOURCE_DIR}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${RooFitUtils_SOURCE_DIR} -DHAS_RooFitUtils")
  set(OPTIONAL_ROOFITUTILS "RooFitUtils")
  message("using RooFitUtils component")
else()
  set(OPTIONAL_ROOFITUTILS "")
  message("using standalone SFramework")
endif()

IF(NOT WITHOUT_ASG)
  atlas_depends_on_subdirs(
    PUBLIC
    QFramework
    ${OPTIONAL_ROOFITUTILS}
    )
ENDIF()

include_directories ("${PROJECT_SOURCE_DIR}")
find_package( ROOT COMPONENTS Core MathCore Gpad Graf Matrix HistPainter HistFactory RooFitCore RooFit RooStats )

# set some variables for easier handling
set(SFrameworkLinkDef ${CMAKE_CURRENT_BINARY_DIR}/LinkDef.h)
set(SFrameworkPython ${CMAKE_CURRENT_BINARY_DIR}/__init__.py)
file(GLOB SFrameworkSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} Root/[A-Za-z]*.cxx)
file(GLOB SFrameworkHeaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} SFramework/[A-Za-z]*.h)
file(GLOB SFrameworkPythonTests "test/[A-Za-z]*.py")

# generate the LinkDef.h
execute_process(
  COMMAND python ${TQPATH}/share/generateBindings.py --set-working-directory ${CMAKE_CURRENT_SOURCE_DIR} --pkgname SFramework --verbose --python ${SFrameworkPython} --python-head  python/SFramework.py --linkdef ${SFrameworkLinkDef} --headers ${SFrameworkHeaders}
)

if(NOT WITHOUT_ASG)
  # this section reflects the standard ASG way of configuring CMake
  # it is executed when compiling within an ASG environment
  find_package( GTest )
  atlas_add_root_dictionary( SFramework SFrameworkCintDict
    ROOT_HEADERS ${SFrameworkHeaders} ${SFrameworkLinkDef}
    EXTERNAL_PACKAGES ROOT )
  atlas_add_library( SFramework
    ${SFrameworkHeaders} ${SFrameworkSources} ${SFrameworkCintDict}
    PUBLIC_HEADERS SFramework
    PRIVATE_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
    LINK_LIBRARIES QFramework ${OPTIONAL_ROOFITUTILS}
    PRIVATE_LINK_LIBRARIES ${ROOT_LIBRARIES} ${LIBXML2_LIBRARIES}
    )

  atlas_install_python_modules( python/*.py )

  foreach(TestScript ${SFrameworkPythonTests})
    get_filename_component(TestName ${TestScript} NAME_WE)
    atlas_add_test( ${TestName} SCRIPT ${TestScript} )
  endforeach()
ELSE()
  # this section is a standalone compilation segment
  # it is executed when compiling outside an ASG environment
  file(APPEND ${CAFSETUP} "export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:${CMAKE_CURRENT_BINARY_DIR}/\n")
  file(APPEND ${CAFSETUP} "export DYLD_LIBRARY_PATH=\${DYLD_LIBRARY_PATH}:${CMAKE_CURRENT_BINARY_DIR}/\n")
  file(APPEND ${CAFSETUP} "export CAF_LIBRARIES=\${CAF_LIBRARIES}:${CMAKE_CURRENT_BINARY_DIR}/\n")

  # register all the files and directories
  include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")
  include_directories ("${ROOT_INCLUDE_DIRS}")
  include_directories ("${TQPATH}")

  if(${RooFitUtils_FOUND})
    include_directories ("${RooFitUtils_SOURCE_DIR}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${RooFitUtils_SOURCE_DIR} -DHAS_RooFitUtils")
		file(APPEND ${CAFSETUP} "export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:${RooFitUtils_BINARY_DIR}/\n")
		file(APPEND ${CAFSETUP} "export DYLD_LIBRARY_PATH=\${DYLD_LIBRARY_PATH}:${RooFitUtils_BINARY_DIR}/\n")
		file(APPEND ${CAFSETUP} "export CAF_LIBRARIES=\${CAF_LIBRARIES}:${RooFitUtils_BINARY_DIR}/\n")
  endif()

  # generate the dictionary source code
  ROOT_GENERATE_DICTIONARY(G__SFramework ${SFrameworkHeaders} LINKDEF ${SFrameworkLinkDef})

  # register the shared object to include both sources and dictionaries
  add_library( SFramework SHARED ${SFrameworkSources} G__SFramework.cxx)

  # link everything together at the end
  target_link_libraries( SFramework QFramework ${OPTIONAL_ROOFITUTILS} ${ROOT_LIBRARIES})

  # register the test cases
  foreach(TestScript ${SFrameworkPythonTests})
    get_filename_component(TestName ${TestScript} NAME_WE)
    add_test( NAME ${TestName} COMMAND ${TestScript})
  endforeach()
ENDIF()
