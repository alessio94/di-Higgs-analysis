# Install script for directory: /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CommonAnalysisHelpers

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/InstallArea/x86_64-centos7-gcc8-opt")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/cvmfs/sft.cern.ch/lcg/releases/binutils/2.30-e5b21/x86_64-centos7/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/src/CAFCore/CommonAnalysisHelpers" TYPE DIRECTORY FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CommonAnalysisHelpers/" USE_SOURCE_PERMISSIONS REGEX "/\\.svn$" EXCLUDE REGEX "/\\.git$" EXCLUDE REGEX "/[^/]*\\~$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers" TYPE FILE RENAME "__init__.py" FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CommonAnalysisHelpers/python/__init__.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers" TYPE FILE RENAME "analyze.py" FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CommonAnalysisHelpers/python/analyze.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers" TYPE DIRECTORY FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CommonAnalysisHelpers/python/analyzeAux" USE_SOURCE_PERMISSIONS REGEX "/\\.svn$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers" TYPE FILE RENAME "common.py" FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CommonAnalysisHelpers/python/common.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers" TYPE FILE RENAME "initialize.py" FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CommonAnalysisHelpers/python/initialize.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers" TYPE FILE RENAME "prepare.py" FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CommonAnalysisHelpers/python/prepare.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers" TYPE FILE RENAME "submit.py" FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CommonAnalysisHelpers/python/submit.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers" TYPE FILE RENAME "visualize.py" FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/CommonAnalysisHelpers/python/visualize.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/CommonAnalysisHelpers/__init__.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/CommonAnalysisHelpers/analyze.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers/analyzeAux" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/CommonAnalysisHelpers/CMakeFiles/pythonBytecode/python/analyzeAux/__init__.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers/analyzeAux" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/CommonAnalysisHelpers/CMakeFiles/pythonBytecode/python/analyzeAux/pycut.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/CommonAnalysisHelpers/common.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/CommonAnalysisHelpers/initialize.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/CommonAnalysisHelpers/prepare.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/CommonAnalysisHelpers/submit.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/CommonAnalysisHelpers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/CommonAnalysisHelpers/visualize.pyc")
endif()

