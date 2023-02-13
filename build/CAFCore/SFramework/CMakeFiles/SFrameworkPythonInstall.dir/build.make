# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake

# The command to remove a file.
RM = /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build

# Utility rule file for SFrameworkPythonInstall.

# Include the progress variables for this target.
include CAFCore/SFramework/CMakeFiles/SFrameworkPythonInstall.dir/progress.make

x86_64-centos7-gcc8-opt/python/SFramework/SFramework.py:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ../../x86_64-centos7-gcc8-opt/python/SFramework/SFramework.py"
	cd /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/SFramework && /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake -E make_directory /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/SFramework
	cd /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/SFramework && /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake -E create_symlink ../../../../CAFCore/SFramework/python/SFramework.py /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/SFramework/SFramework.py

x86_64-centos7-gcc8-opt/python/SFramework/libSFramework.py:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating ../../x86_64-centos7-gcc8-opt/python/SFramework/libSFramework.py"
	cd /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/SFramework && /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake -E make_directory /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/SFramework
	cd /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/SFramework && /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake -E create_symlink ../../../../CAFCore/SFramework/python/libSFramework.py /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/python/SFramework/libSFramework.py

SFrameworkPythonInstall: x86_64-centos7-gcc8-opt/python/SFramework/SFramework.py
SFrameworkPythonInstall: x86_64-centos7-gcc8-opt/python/SFramework/libSFramework.py
SFrameworkPythonInstall: CAFCore/SFramework/CMakeFiles/SFrameworkPythonInstall.dir/build.make

.PHONY : SFrameworkPythonInstall

# Rule to build all files generated by this target.
CAFCore/SFramework/CMakeFiles/SFrameworkPythonInstall.dir/build: SFrameworkPythonInstall

.PHONY : CAFCore/SFramework/CMakeFiles/SFrameworkPythonInstall.dir/build

CAFCore/SFramework/CMakeFiles/SFrameworkPythonInstall.dir/clean:
	cd /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/SFramework && $(CMAKE_COMMAND) -P CMakeFiles/SFrameworkPythonInstall.dir/cmake_clean.cmake
.PHONY : CAFCore/SFramework/CMakeFiles/SFrameworkPythonInstall.dir/clean

CAFCore/SFramework/CMakeFiles/SFrameworkPythonInstall.dir/depend:
	cd /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CAFCore/SFramework /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/SFramework /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CAFCore/SFramework/CMakeFiles/SFrameworkPythonInstall.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CAFCore/SFramework/CMakeFiles/SFrameworkPythonInstall.dir/depend
