# Install script for directory: /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/SubmissionHelpers

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE DIRECTORY FILES "/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/cmake/modules" USE_SOURCE_PERMISSIONS REGEX "/\\.svn$" EXCLUDE REGEX "/[^/]*\\~$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES
    "/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/cmake/LCGConfig.cmake"
    "/cvmfs/atlas.cern.ch/repo/sw/software/21.2/AnalysisBase/21.2.174/InstallArea/x86_64-centos7-gcc8-opt/cmake/LCGConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/src/SubmissionHelpers" TYPE DIRECTORY FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/SubmissionHelpers/" USE_SOURCE_PERMISSIONS REGEX "/\\.svn$" EXCLUDE REGEX "/\\.git$" EXCLUDE REGEX "/[^/]*\\~$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers" TYPE FILE RENAME "__init__.py" FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/SubmissionHelpers/python/__init__.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers" TYPE FILE RENAME "common.py" FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/SubmissionHelpers/python/common.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers" TYPE FILE RENAME "submissionControllerBase.py" FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/SubmissionHelpers/python/submissionControllerBase.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers" TYPE DIRECTORY FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/SubmissionHelpers/python/submissionControllers" USE_SOURCE_PERMISSIONS REGEX "/\\.svn$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers" TYPE FILE RENAME "submit.py" FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/SubmissionHelpers/python/submit.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers" TYPE FILE RENAME "task.py" FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/SubmissionHelpers/python/task.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/SubmissionHelpers/__init__.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/SubmissionHelpers/common.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/SubmissionHelpers/submissionControllerBase.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers/submissionControllers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/SubmissionHelpers/CMakeFiles/pythonBytecode/python/submissionControllers/__init__.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers/submissionControllers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/SubmissionHelpers/CMakeFiles/pythonBytecode/python/submissionControllers/condor.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers/submissionControllers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/SubmissionHelpers/CMakeFiles/pythonBytecode/python/submissionControllers/kubernetes.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers/submissionControllers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/SubmissionHelpers/CMakeFiles/pythonBytecode/python/submissionControllers/local.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers/submissionControllers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/SubmissionHelpers/CMakeFiles/pythonBytecode/python/submissionControllers/moab.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers/submissionControllers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/SubmissionHelpers/CMakeFiles/pythonBytecode/python/submissionControllers/sge.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers/submissionControllers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/SubmissionHelpers/CMakeFiles/pythonBytecode/python/submissionControllers/slurm.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers/submissionControllers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/SubmissionHelpers/CMakeFiles/pythonBytecode/python/submissionControllers/slurmOnCedar.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers/submissionControllers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/SubmissionHelpers/CMakeFiles/pythonBytecode/python/submissionControllers/torque.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/SubmissionHelpers/submit.pyc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xMainx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/SubmissionHelpers" TYPE FILE FILES "/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/SubmissionHelpers/task.pyc")
endif()

