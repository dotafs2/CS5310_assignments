# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.28

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

# Produce verbose output by default.
VERBOSE = 1

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = D:\CLion\bin\cmake\win\x64\bin\cmake.exe

# The command to remove a file.
RM = D:\CLion\bin\cmake\win\x64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = F:\CS5310_assignments

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = F:\CS5310_assignments\cmake-build-debug

# Utility rule file for test6b_autogen_timestamp_deps.

# Include any custom commands dependencies for this target.
include CMakeFiles/test6b_autogen_timestamp_deps.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test6b_autogen_timestamp_deps.dir/progress.make

CMakeFiles/test6b_autogen_timestamp_deps: C:/Qt/6.7.1/mingw_64/./bin/uic.exe
CMakeFiles/test6b_autogen_timestamp_deps: C:/Qt/6.7.1/mingw_64/lib/libQt6Core.a
CMakeFiles/test6b_autogen_timestamp_deps: C:/Qt/6.7.1/mingw_64/./bin/moc.exe
CMakeFiles/test6b_autogen_timestamp_deps: C:/Qt/6.7.1/mingw_64/lib/libQt6Widgets.a

test6b_autogen_timestamp_deps: CMakeFiles/test6b_autogen_timestamp_deps
test6b_autogen_timestamp_deps: CMakeFiles/test6b_autogen_timestamp_deps.dir/build.make
.PHONY : test6b_autogen_timestamp_deps

# Rule to build all files generated by this target.
CMakeFiles/test6b_autogen_timestamp_deps.dir/build: test6b_autogen_timestamp_deps
.PHONY : CMakeFiles/test6b_autogen_timestamp_deps.dir/build

CMakeFiles/test6b_autogen_timestamp_deps.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\test6b_autogen_timestamp_deps.dir\cmake_clean.cmake
.PHONY : CMakeFiles/test6b_autogen_timestamp_deps.dir/clean

CMakeFiles/test6b_autogen_timestamp_deps.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" F:\CS5310_assignments F:\CS5310_assignments F:\CS5310_assignments\cmake-build-debug F:\CS5310_assignments\cmake-build-debug F:\CS5310_assignments\cmake-build-debug\CMakeFiles\test6b_autogen_timestamp_deps.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/test6b_autogen_timestamp_deps.dir/depend

