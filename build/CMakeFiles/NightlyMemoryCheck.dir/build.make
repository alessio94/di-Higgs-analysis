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

# Utility rule file for NightlyMemoryCheck.

# Include the progress variables for this target.
include CMakeFiles/NightlyMemoryCheck.dir/progress.make

CMakeFiles/NightlyMemoryCheck:
	/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Cmake/3.18.3/Linux-x86_64/bin/ctest -D NightlyMemoryCheck

NightlyMemoryCheck: CMakeFiles/NightlyMemoryCheck
NightlyMemoryCheck: CMakeFiles/NightlyMemoryCheck.dir/build.make

.PHONY : NightlyMemoryCheck

# Rule to build all files generated by this target.
CMakeFiles/NightlyMemoryCheck.dir/build: NightlyMemoryCheck

.PHONY : CMakeFiles/NightlyMemoryCheck.dir/build

CMakeFiles/NightlyMemoryCheck.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/NightlyMemoryCheck.dir/cmake_clean.cmake
.PHONY : CMakeFiles/NightlyMemoryCheck.dir/clean

CMakeFiles/NightlyMemoryCheck.dir/depend:
	cd /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build /afs/cern.ch/work/a/apizzini/private/2022/nov/CAFbbll/build/CMakeFiles/NightlyMemoryCheck.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/NightlyMemoryCheck.dir/depend

