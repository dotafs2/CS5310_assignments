# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/dotafs/Desktop/CS5310_assignments/assignment5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/test6.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test6.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test6.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test6.dir/flags.make

CMakeFiles/test6.dir/test6_autogen/mocs_compilation.cpp.o: CMakeFiles/test6.dir/flags.make
CMakeFiles/test6.dir/test6_autogen/mocs_compilation.cpp.o: test6_autogen/mocs_compilation.cpp
CMakeFiles/test6.dir/test6_autogen/mocs_compilation.cpp.o: CMakeFiles/test6.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test6.dir/test6_autogen/mocs_compilation.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test6.dir/test6_autogen/mocs_compilation.cpp.o -MF CMakeFiles/test6.dir/test6_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/test6.dir/test6_autogen/mocs_compilation.cpp.o -c /Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug/test6_autogen/mocs_compilation.cpp

CMakeFiles/test6.dir/test6_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test6.dir/test6_autogen/mocs_compilation.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug/test6_autogen/mocs_compilation.cpp > CMakeFiles/test6.dir/test6_autogen/mocs_compilation.cpp.i

CMakeFiles/test6.dir/test6_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test6.dir/test6_autogen/mocs_compilation.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug/test6_autogen/mocs_compilation.cpp -o CMakeFiles/test6.dir/test6_autogen/mocs_compilation.cpp.s

CMakeFiles/test6.dir/src/Image.c.o: CMakeFiles/test6.dir/flags.make
CMakeFiles/test6.dir/src/Image.c.o: /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Image.c
CMakeFiles/test6.dir/src/Image.c.o: CMakeFiles/test6.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/test6.dir/src/Image.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test6.dir/src/Image.c.o -MF CMakeFiles/test6.dir/src/Image.c.o.d -o CMakeFiles/test6.dir/src/Image.c.o -c /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Image.c

CMakeFiles/test6.dir/src/Image.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test6.dir/src/Image.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Image.c > CMakeFiles/test6.dir/src/Image.c.i

CMakeFiles/test6.dir/src/Image.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test6.dir/src/Image.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Image.c -o CMakeFiles/test6.dir/src/Image.c.s

CMakeFiles/test6.dir/src/Line.c.o: CMakeFiles/test6.dir/flags.make
CMakeFiles/test6.dir/src/Line.c.o: /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Line.c
CMakeFiles/test6.dir/src/Line.c.o: CMakeFiles/test6.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/test6.dir/src/Line.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test6.dir/src/Line.c.o -MF CMakeFiles/test6.dir/src/Line.c.o.d -o CMakeFiles/test6.dir/src/Line.c.o -c /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Line.c

CMakeFiles/test6.dir/src/Line.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test6.dir/src/Line.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Line.c > CMakeFiles/test6.dir/src/Line.c.i

CMakeFiles/test6.dir/src/Line.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test6.dir/src/Line.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Line.c -o CMakeFiles/test6.dir/src/Line.c.s

CMakeFiles/test6.dir/src/anti-alias.cpp.o: CMakeFiles/test6.dir/flags.make
CMakeFiles/test6.dir/src/anti-alias.cpp.o: /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/anti-alias.cpp
CMakeFiles/test6.dir/src/anti-alias.cpp.o: CMakeFiles/test6.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/test6.dir/src/anti-alias.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) -x c++ $(CXX_FLAGS) -MD -MT CMakeFiles/test6.dir/src/anti-alias.cpp.o -MF CMakeFiles/test6.dir/src/anti-alias.cpp.o.d -o CMakeFiles/test6.dir/src/anti-alias.cpp.o -c /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/anti-alias.cpp

CMakeFiles/test6.dir/src/anti-alias.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test6.dir/src/anti-alias.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) -x c++ $(CXX_FLAGS) -E /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/anti-alias.cpp > CMakeFiles/test6.dir/src/anti-alias.cpp.i

CMakeFiles/test6.dir/src/anti-alias.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test6.dir/src/anti-alias.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) -x c++ $(CXX_FLAGS) -S /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/anti-alias.cpp -o CMakeFiles/test6.dir/src/anti-alias.cpp.s

CMakeFiles/test6.dir/src/ball.cpp.o: CMakeFiles/test6.dir/flags.make
CMakeFiles/test6.dir/src/ball.cpp.o: /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/ball.cpp
CMakeFiles/test6.dir/src/ball.cpp.o: CMakeFiles/test6.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/test6.dir/src/ball.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) -x c++ $(CXX_FLAGS) -MD -MT CMakeFiles/test6.dir/src/ball.cpp.o -MF CMakeFiles/test6.dir/src/ball.cpp.o.d -o CMakeFiles/test6.dir/src/ball.cpp.o -c /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/ball.cpp

CMakeFiles/test6.dir/src/ball.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test6.dir/src/ball.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) -x c++ $(CXX_FLAGS) -E /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/ball.cpp > CMakeFiles/test6.dir/src/ball.cpp.i

CMakeFiles/test6.dir/src/ball.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test6.dir/src/ball.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) -x c++ $(CXX_FLAGS) -S /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/ball.cpp -o CMakeFiles/test6.dir/src/ball.cpp.s

CMakeFiles/test6.dir/src/Polygon.c.o: CMakeFiles/test6.dir/flags.make
CMakeFiles/test6.dir/src/Polygon.c.o: /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Polygon.c
CMakeFiles/test6.dir/src/Polygon.c.o: CMakeFiles/test6.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/test6.dir/src/Polygon.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test6.dir/src/Polygon.c.o -MF CMakeFiles/test6.dir/src/Polygon.c.o.d -o CMakeFiles/test6.dir/src/Polygon.c.o -c /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Polygon.c

CMakeFiles/test6.dir/src/Polygon.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test6.dir/src/Polygon.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Polygon.c > CMakeFiles/test6.dir/src/Polygon.c.i

CMakeFiles/test6.dir/src/Polygon.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test6.dir/src/Polygon.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Polygon.c -o CMakeFiles/test6.dir/src/Polygon.c.s

CMakeFiles/test6.dir/src/fsMath.c.o: CMakeFiles/test6.dir/flags.make
CMakeFiles/test6.dir/src/fsMath.c.o: /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/fsMath.c
CMakeFiles/test6.dir/src/fsMath.c.o: CMakeFiles/test6.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/test6.dir/src/fsMath.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test6.dir/src/fsMath.c.o -MF CMakeFiles/test6.dir/src/fsMath.c.o.d -o CMakeFiles/test6.dir/src/fsMath.c.o -c /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/fsMath.c

CMakeFiles/test6.dir/src/fsMath.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test6.dir/src/fsMath.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/fsMath.c > CMakeFiles/test6.dir/src/fsMath.c.i

CMakeFiles/test6.dir/src/fsMath.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test6.dir/src/fsMath.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/fsMath.c -o CMakeFiles/test6.dir/src/fsMath.c.s

CMakeFiles/test6.dir/src/Module.c.o: CMakeFiles/test6.dir/flags.make
CMakeFiles/test6.dir/src/Module.c.o: /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Module.c
CMakeFiles/test6.dir/src/Module.c.o: CMakeFiles/test6.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/test6.dir/src/Module.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test6.dir/src/Module.c.o -MF CMakeFiles/test6.dir/src/Module.c.o.d -o CMakeFiles/test6.dir/src/Module.c.o -c /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Module.c

CMakeFiles/test6.dir/src/Module.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test6.dir/src/Module.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Module.c > CMakeFiles/test6.dir/src/Module.c.i

CMakeFiles/test6.dir/src/Module.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test6.dir/src/Module.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/dotafs/Desktop/CS5310_assignments/assignment5/src/Module.c -o CMakeFiles/test6.dir/src/Module.c.s

CMakeFiles/test6.dir/test/test6/test6a.c.o: CMakeFiles/test6.dir/flags.make
CMakeFiles/test6.dir/test/test6/test6a.c.o: /Users/dotafs/Desktop/CS5310_assignments/assignment5/test/test6/test6a.c
CMakeFiles/test6.dir/test/test6/test6a.c.o: CMakeFiles/test6.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/test6.dir/test/test6/test6a.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test6.dir/test/test6/test6a.c.o -MF CMakeFiles/test6.dir/test/test6/test6a.c.o.d -o CMakeFiles/test6.dir/test/test6/test6a.c.o -c /Users/dotafs/Desktop/CS5310_assignments/assignment5/test/test6/test6a.c

CMakeFiles/test6.dir/test/test6/test6a.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test6.dir/test/test6/test6a.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/dotafs/Desktop/CS5310_assignments/assignment5/test/test6/test6a.c > CMakeFiles/test6.dir/test/test6/test6a.c.i

CMakeFiles/test6.dir/test/test6/test6a.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test6.dir/test/test6/test6a.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/dotafs/Desktop/CS5310_assignments/assignment5/test/test6/test6a.c -o CMakeFiles/test6.dir/test/test6/test6a.c.s

# Object files for target test6
test6_OBJECTS = \
"CMakeFiles/test6.dir/test6_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/test6.dir/src/Image.c.o" \
"CMakeFiles/test6.dir/src/Line.c.o" \
"CMakeFiles/test6.dir/src/anti-alias.cpp.o" \
"CMakeFiles/test6.dir/src/ball.cpp.o" \
"CMakeFiles/test6.dir/src/Polygon.c.o" \
"CMakeFiles/test6.dir/src/fsMath.c.o" \
"CMakeFiles/test6.dir/src/Module.c.o" \
"CMakeFiles/test6.dir/test/test6/test6a.c.o"

# External object files for target test6
test6_EXTERNAL_OBJECTS =

test6: CMakeFiles/test6.dir/test6_autogen/mocs_compilation.cpp.o
test6: CMakeFiles/test6.dir/src/Image.c.o
test6: CMakeFiles/test6.dir/src/Line.c.o
test6: CMakeFiles/test6.dir/src/anti-alias.cpp.o
test6: CMakeFiles/test6.dir/src/ball.cpp.o
test6: CMakeFiles/test6.dir/src/Polygon.c.o
test6: CMakeFiles/test6.dir/src/fsMath.c.o
test6: CMakeFiles/test6.dir/src/Module.c.o
test6: CMakeFiles/test6.dir/test/test6/test6a.c.o
test6: CMakeFiles/test6.dir/build.make
test6: /Users/dotafs/Qt/6.7.2/macos/lib/QtWidgets.framework/Versions/A/QtWidgets
test6: /Users/dotafs/Qt/6.7.2/macos/lib/QtGui.framework/Versions/A/QtGui
test6: /Users/dotafs/Qt/6.7.2/macos/lib/QtCore.framework/Versions/A/QtCore
test6: CMakeFiles/test6.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable test6"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test6.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test6.dir/build: test6
.PHONY : CMakeFiles/test6.dir/build

CMakeFiles/test6.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test6.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test6.dir/clean

CMakeFiles/test6.dir/depend:
	cd /Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/dotafs/Desktop/CS5310_assignments/assignment5 /Users/dotafs/Desktop/CS5310_assignments/assignment5 /Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug /Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug /Users/dotafs/Desktop/CS5310_assignments/assignment5/cmake-build-debug/CMakeFiles/test6.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/test6.dir/depend

