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

# Include any dependencies generated for this target.
include CMakeFiles/test6a.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test6a.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test6a.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test6a.dir/flags.make

CMakeFiles/test6a.dir/test6a_autogen/mocs_compilation.cpp.obj: CMakeFiles/test6a.dir/flags.make
CMakeFiles/test6a.dir/test6a_autogen/mocs_compilation.cpp.obj: CMakeFiles/test6a.dir/includes_CXX.rsp
CMakeFiles/test6a.dir/test6a_autogen/mocs_compilation.cpp.obj: test6a_autogen/mocs_compilation.cpp
CMakeFiles/test6a.dir/test6a_autogen/mocs_compilation.cpp.obj: CMakeFiles/test6a.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=F:\CS5310_assignments\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test6a.dir/test6a_autogen/mocs_compilation.cpp.obj"
	D:\CLion\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test6a.dir/test6a_autogen/mocs_compilation.cpp.obj -MF CMakeFiles\test6a.dir\test6a_autogen\mocs_compilation.cpp.obj.d -o CMakeFiles\test6a.dir\test6a_autogen\mocs_compilation.cpp.obj -c F:\CS5310_assignments\cmake-build-debug\test6a_autogen\mocs_compilation.cpp

CMakeFiles/test6a.dir/test6a_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test6a.dir/test6a_autogen/mocs_compilation.cpp.i"
	D:\CLion\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E F:\CS5310_assignments\cmake-build-debug\test6a_autogen\mocs_compilation.cpp > CMakeFiles\test6a.dir\test6a_autogen\mocs_compilation.cpp.i

CMakeFiles/test6a.dir/test6a_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test6a.dir/test6a_autogen/mocs_compilation.cpp.s"
	D:\CLion\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S F:\CS5310_assignments\cmake-build-debug\test6a_autogen\mocs_compilation.cpp -o CMakeFiles\test6a.dir\test6a_autogen\mocs_compilation.cpp.s

CMakeFiles/test6a.dir/src/Image.c.obj: CMakeFiles/test6a.dir/flags.make
CMakeFiles/test6a.dir/src/Image.c.obj: CMakeFiles/test6a.dir/includes_C.rsp
CMakeFiles/test6a.dir/src/Image.c.obj: F:/CS5310_assignments/src/Image.c
CMakeFiles/test6a.dir/src/Image.c.obj: CMakeFiles/test6a.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=F:\CS5310_assignments\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/test6a.dir/src/Image.c.obj"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test6a.dir/src/Image.c.obj -MF CMakeFiles\test6a.dir\src\Image.c.obj.d -o CMakeFiles\test6a.dir\src\Image.c.obj -c F:\CS5310_assignments\src\Image.c

CMakeFiles/test6a.dir/src/Image.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test6a.dir/src/Image.c.i"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E F:\CS5310_assignments\src\Image.c > CMakeFiles\test6a.dir\src\Image.c.i

CMakeFiles/test6a.dir/src/Image.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test6a.dir/src/Image.c.s"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S F:\CS5310_assignments\src\Image.c -o CMakeFiles\test6a.dir\src\Image.c.s

CMakeFiles/test6a.dir/src/Line.c.obj: CMakeFiles/test6a.dir/flags.make
CMakeFiles/test6a.dir/src/Line.c.obj: CMakeFiles/test6a.dir/includes_C.rsp
CMakeFiles/test6a.dir/src/Line.c.obj: F:/CS5310_assignments/src/Line.c
CMakeFiles/test6a.dir/src/Line.c.obj: CMakeFiles/test6a.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=F:\CS5310_assignments\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/test6a.dir/src/Line.c.obj"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test6a.dir/src/Line.c.obj -MF CMakeFiles\test6a.dir\src\Line.c.obj.d -o CMakeFiles\test6a.dir\src\Line.c.obj -c F:\CS5310_assignments\src\Line.c

CMakeFiles/test6a.dir/src/Line.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test6a.dir/src/Line.c.i"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E F:\CS5310_assignments\src\Line.c > CMakeFiles\test6a.dir\src\Line.c.i

CMakeFiles/test6a.dir/src/Line.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test6a.dir/src/Line.c.s"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S F:\CS5310_assignments\src\Line.c -o CMakeFiles\test6a.dir\src\Line.c.s

CMakeFiles/test6a.dir/src/anti-alias.cpp.obj: CMakeFiles/test6a.dir/flags.make
CMakeFiles/test6a.dir/src/anti-alias.cpp.obj: CMakeFiles/test6a.dir/includes_CXX.rsp
CMakeFiles/test6a.dir/src/anti-alias.cpp.obj: F:/CS5310_assignments/src/anti-alias.cpp
CMakeFiles/test6a.dir/src/anti-alias.cpp.obj: CMakeFiles/test6a.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=F:\CS5310_assignments\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/test6a.dir/src/anti-alias.cpp.obj"
	D:\CLion\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) -x c++ $(CXX_FLAGS) -MD -MT CMakeFiles/test6a.dir/src/anti-alias.cpp.obj -MF CMakeFiles\test6a.dir\src\anti-alias.cpp.obj.d -o CMakeFiles\test6a.dir\src\anti-alias.cpp.obj -c F:\CS5310_assignments\src\anti-alias.cpp

CMakeFiles/test6a.dir/src/anti-alias.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test6a.dir/src/anti-alias.cpp.i"
	D:\CLion\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) -x c++ $(CXX_FLAGS) -E F:\CS5310_assignments\src\anti-alias.cpp > CMakeFiles\test6a.dir\src\anti-alias.cpp.i

CMakeFiles/test6a.dir/src/anti-alias.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test6a.dir/src/anti-alias.cpp.s"
	D:\CLion\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) -x c++ $(CXX_FLAGS) -S F:\CS5310_assignments\src\anti-alias.cpp -o CMakeFiles\test6a.dir\src\anti-alias.cpp.s

CMakeFiles/test6a.dir/src/ball.cpp.obj: CMakeFiles/test6a.dir/flags.make
CMakeFiles/test6a.dir/src/ball.cpp.obj: CMakeFiles/test6a.dir/includes_CXX.rsp
CMakeFiles/test6a.dir/src/ball.cpp.obj: F:/CS5310_assignments/src/ball.cpp
CMakeFiles/test6a.dir/src/ball.cpp.obj: CMakeFiles/test6a.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=F:\CS5310_assignments\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/test6a.dir/src/ball.cpp.obj"
	D:\CLion\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) -x c++ $(CXX_FLAGS) -MD -MT CMakeFiles/test6a.dir/src/ball.cpp.obj -MF CMakeFiles\test6a.dir\src\ball.cpp.obj.d -o CMakeFiles\test6a.dir\src\ball.cpp.obj -c F:\CS5310_assignments\src\ball.cpp

CMakeFiles/test6a.dir/src/ball.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test6a.dir/src/ball.cpp.i"
	D:\CLion\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) -x c++ $(CXX_FLAGS) -E F:\CS5310_assignments\src\ball.cpp > CMakeFiles\test6a.dir\src\ball.cpp.i

CMakeFiles/test6a.dir/src/ball.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test6a.dir/src/ball.cpp.s"
	D:\CLion\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) -x c++ $(CXX_FLAGS) -S F:\CS5310_assignments\src\ball.cpp -o CMakeFiles\test6a.dir\src\ball.cpp.s

CMakeFiles/test6a.dir/src/Polygon.c.obj: CMakeFiles/test6a.dir/flags.make
CMakeFiles/test6a.dir/src/Polygon.c.obj: CMakeFiles/test6a.dir/includes_C.rsp
CMakeFiles/test6a.dir/src/Polygon.c.obj: F:/CS5310_assignments/src/Polygon.c
CMakeFiles/test6a.dir/src/Polygon.c.obj: CMakeFiles/test6a.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=F:\CS5310_assignments\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/test6a.dir/src/Polygon.c.obj"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test6a.dir/src/Polygon.c.obj -MF CMakeFiles\test6a.dir\src\Polygon.c.obj.d -o CMakeFiles\test6a.dir\src\Polygon.c.obj -c F:\CS5310_assignments\src\Polygon.c

CMakeFiles/test6a.dir/src/Polygon.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test6a.dir/src/Polygon.c.i"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E F:\CS5310_assignments\src\Polygon.c > CMakeFiles\test6a.dir\src\Polygon.c.i

CMakeFiles/test6a.dir/src/Polygon.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test6a.dir/src/Polygon.c.s"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S F:\CS5310_assignments\src\Polygon.c -o CMakeFiles\test6a.dir\src\Polygon.c.s

CMakeFiles/test6a.dir/src/fsMath.c.obj: CMakeFiles/test6a.dir/flags.make
CMakeFiles/test6a.dir/src/fsMath.c.obj: CMakeFiles/test6a.dir/includes_C.rsp
CMakeFiles/test6a.dir/src/fsMath.c.obj: F:/CS5310_assignments/src/fsMath.c
CMakeFiles/test6a.dir/src/fsMath.c.obj: CMakeFiles/test6a.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=F:\CS5310_assignments\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/test6a.dir/src/fsMath.c.obj"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test6a.dir/src/fsMath.c.obj -MF CMakeFiles\test6a.dir\src\fsMath.c.obj.d -o CMakeFiles\test6a.dir\src\fsMath.c.obj -c F:\CS5310_assignments\src\fsMath.c

CMakeFiles/test6a.dir/src/fsMath.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test6a.dir/src/fsMath.c.i"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E F:\CS5310_assignments\src\fsMath.c > CMakeFiles\test6a.dir\src\fsMath.c.i

CMakeFiles/test6a.dir/src/fsMath.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test6a.dir/src/fsMath.c.s"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S F:\CS5310_assignments\src\fsMath.c -o CMakeFiles\test6a.dir\src\fsMath.c.s

CMakeFiles/test6a.dir/src/Module.c.obj: CMakeFiles/test6a.dir/flags.make
CMakeFiles/test6a.dir/src/Module.c.obj: CMakeFiles/test6a.dir/includes_C.rsp
CMakeFiles/test6a.dir/src/Module.c.obj: F:/CS5310_assignments/src/Module.c
CMakeFiles/test6a.dir/src/Module.c.obj: CMakeFiles/test6a.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=F:\CS5310_assignments\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/test6a.dir/src/Module.c.obj"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test6a.dir/src/Module.c.obj -MF CMakeFiles\test6a.dir\src\Module.c.obj.d -o CMakeFiles\test6a.dir\src\Module.c.obj -c F:\CS5310_assignments\src\Module.c

CMakeFiles/test6a.dir/src/Module.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test6a.dir/src/Module.c.i"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E F:\CS5310_assignments\src\Module.c > CMakeFiles\test6a.dir\src\Module.c.i

CMakeFiles/test6a.dir/src/Module.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test6a.dir/src/Module.c.s"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S F:\CS5310_assignments\src\Module.c -o CMakeFiles\test6a.dir\src\Module.c.s

CMakeFiles/test6a.dir/src/Bezier.c.obj: CMakeFiles/test6a.dir/flags.make
CMakeFiles/test6a.dir/src/Bezier.c.obj: CMakeFiles/test6a.dir/includes_C.rsp
CMakeFiles/test6a.dir/src/Bezier.c.obj: F:/CS5310_assignments/src/Bezier.c
CMakeFiles/test6a.dir/src/Bezier.c.obj: CMakeFiles/test6a.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=F:\CS5310_assignments\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/test6a.dir/src/Bezier.c.obj"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test6a.dir/src/Bezier.c.obj -MF CMakeFiles\test6a.dir\src\Bezier.c.obj.d -o CMakeFiles\test6a.dir\src\Bezier.c.obj -c F:\CS5310_assignments\src\Bezier.c

CMakeFiles/test6a.dir/src/Bezier.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test6a.dir/src/Bezier.c.i"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E F:\CS5310_assignments\src\Bezier.c > CMakeFiles\test6a.dir\src\Bezier.c.i

CMakeFiles/test6a.dir/src/Bezier.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test6a.dir/src/Bezier.c.s"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S F:\CS5310_assignments\src\Bezier.c -o CMakeFiles\test6a.dir\src\Bezier.c.s

CMakeFiles/test6a.dir/test/test5/test5c.c.obj: CMakeFiles/test6a.dir/flags.make
CMakeFiles/test6a.dir/test/test5/test5c.c.obj: CMakeFiles/test6a.dir/includes_C.rsp
CMakeFiles/test6a.dir/test/test5/test5c.c.obj: F:/CS5310_assignments/test/test5/test5c.c
CMakeFiles/test6a.dir/test/test5/test5c.c.obj: CMakeFiles/test6a.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=F:\CS5310_assignments\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/test6a.dir/test/test5/test5c.c.obj"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test6a.dir/test/test5/test5c.c.obj -MF CMakeFiles\test6a.dir\test\test5\test5c.c.obj.d -o CMakeFiles\test6a.dir\test\test5\test5c.c.obj -c F:\CS5310_assignments\test\test5\test5c.c

CMakeFiles/test6a.dir/test/test5/test5c.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test6a.dir/test/test5/test5c.c.i"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E F:\CS5310_assignments\test\test5\test5c.c > CMakeFiles\test6a.dir\test\test5\test5c.c.i

CMakeFiles/test6a.dir/test/test5/test5c.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test6a.dir/test/test5/test5c.c.s"
	D:\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S F:\CS5310_assignments\test\test5\test5c.c -o CMakeFiles\test6a.dir\test\test5\test5c.c.s

# Object files for target test6a
test6a_OBJECTS = \
"CMakeFiles/test6a.dir/test6a_autogen/mocs_compilation.cpp.obj" \
"CMakeFiles/test6a.dir/src/Image.c.obj" \
"CMakeFiles/test6a.dir/src/Line.c.obj" \
"CMakeFiles/test6a.dir/src/anti-alias.cpp.obj" \
"CMakeFiles/test6a.dir/src/ball.cpp.obj" \
"CMakeFiles/test6a.dir/src/Polygon.c.obj" \
"CMakeFiles/test6a.dir/src/fsMath.c.obj" \
"CMakeFiles/test6a.dir/src/Module.c.obj" \
"CMakeFiles/test6a.dir/src/Bezier.c.obj" \
"CMakeFiles/test6a.dir/test/test5/test5c.c.obj"

# External object files for target test6a
test6a_EXTERNAL_OBJECTS =

test6a.exe: CMakeFiles/test6a.dir/test6a_autogen/mocs_compilation.cpp.obj
test6a.exe: CMakeFiles/test6a.dir/src/Image.c.obj
test6a.exe: CMakeFiles/test6a.dir/src/Line.c.obj
test6a.exe: CMakeFiles/test6a.dir/src/anti-alias.cpp.obj
test6a.exe: CMakeFiles/test6a.dir/src/ball.cpp.obj
test6a.exe: CMakeFiles/test6a.dir/src/Polygon.c.obj
test6a.exe: CMakeFiles/test6a.dir/src/fsMath.c.obj
test6a.exe: CMakeFiles/test6a.dir/src/Module.c.obj
test6a.exe: CMakeFiles/test6a.dir/src/Bezier.c.obj
test6a.exe: CMakeFiles/test6a.dir/test/test5/test5c.c.obj
test6a.exe: CMakeFiles/test6a.dir/build.make
test6a.exe: C:/Qt/6.7.1/mingw_64/lib/libQt6Widgets.a
test6a.exe: C:/Qt/6.7.1/mingw_64/lib/libQt6Gui.a
test6a.exe: C:/Qt/6.7.1/mingw_64/lib/libQt6Core.a
test6a.exe: CMakeFiles/test6a.dir/linkLibs.rsp
test6a.exe: CMakeFiles/test6a.dir/objects1.rsp
test6a.exe: CMakeFiles/test6a.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=F:\CS5310_assignments\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable test6a.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\test6a.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test6a.dir/build: test6a.exe
.PHONY : CMakeFiles/test6a.dir/build

CMakeFiles/test6a.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\test6a.dir\cmake_clean.cmake
.PHONY : CMakeFiles/test6a.dir/clean

CMakeFiles/test6a.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" F:\CS5310_assignments F:\CS5310_assignments F:\CS5310_assignments\cmake-build-debug F:\CS5310_assignments\cmake-build-debug F:\CS5310_assignments\cmake-build-debug\CMakeFiles\test6a.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/test6a.dir/depend

