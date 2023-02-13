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

# Utility rule file for CxAODToolsHeaderInstall.

# Include the progress variables for this target.
include CxAODTools/CMakeFiles/CxAODToolsHeaderInstall.dir/progress.make

x86_64-centos7-gcc8-opt/include/CxAODTools:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ../x86_64-centos7-gcc8-opt/include/CxAODTools"
	cd /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CxAODTools && /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake -E make_directory /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/include
	cd /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CxAODTools && /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/cmake -E create_symlink ../../../CxAODTools/CxAODTools /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/x86_64-centos7-gcc8-opt/include/CxAODTools

CxAODToolsHeaderInstall: x86_64-centos7-gcc8-opt/include/CxAODTools
CxAODToolsHeaderInstall: CxAODTools/CMakeFiles/CxAODToolsHeaderInstall.dir/build.make

.PHONY : CxAODToolsHeaderInstall

# Rule to build all files generated by this target.
CxAODTools/CMakeFiles/CxAODToolsHeaderInstall.dir/build: CxAODToolsHeaderInstall

.PHONY : CxAODTools/CMakeFiles/CxAODToolsHeaderInstall.dir/build

CxAODTools/CMakeFiles/CxAODToolsHeaderInstall.dir/clean:
	cd /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CxAODTools && $(CMAKE_COMMAND) -P CMakeFiles/CxAODToolsHeaderInstall.dir/cmake_clean.cmake
.PHONY : CxAODTools/CMakeFiles/CxAODToolsHeaderInstall.dir/clean

CxAODTools/CMakeFiles/CxAODToolsHeaderInstall.dir/depend:
	cd /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/CxAODTools /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CxAODTools /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CxAODTools/CMakeFiles/CxAODToolsHeaderInstall.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CxAODTools/CMakeFiles/CxAODToolsHeaderInstall.dir/depend

