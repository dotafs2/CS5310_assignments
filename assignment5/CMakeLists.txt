cmake_minimum_required(VERSION 3.27)
project(assignment5 C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

# Enable verbose makefile output for debugging
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set the CMAKE_PREFIX_PATH to help CMake locate Qt
set(CMAKE_PREFIX_PATH "C:/Qt/6.7.1/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(include)

set(SOURCE_FILES
        src/Image.c
        src/Line.c
        include/Line.h
        src/anti-alias.cpp
        src/ball.cpp
        src/Polygon.c
        include/Polygon.h
        src/fsMath.c
        include/fsMath.h
        src/main.cpp
        include/mainwindow.h
        src/mainwindow.cpp
)

add_executable(assignment5 ${SOURCE_FILES})

set_source_files_properties(
        src/anti-alias.cpp
        src/ball.cpp
        src/main.cpp
        src/mainwindow.cpp
        PROPERTIES
        LANGUAGE CXX
)

# Link Qt6 Widgets library
target_link_libraries(assignment5 Qt6::Core Qt6::Widgets)
