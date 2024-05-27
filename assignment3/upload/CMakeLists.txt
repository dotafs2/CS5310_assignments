cmake_minimum_required(VERSION 3.27)
project(assignment3 C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

include_directories(include)

add_executable(assignment3
        include/Image.h
        src/Image.c
        include/Line.h
        src/Line.c
        include/anti-alias.h
        src/anti-alias.cpp
        src/mytest.cpp
        src/ball.cpp
        include/ball.h
)

set_source_files_properties(
        src/anti-alias.cpp
        src/mytest.cpp
        src/ball.cpp
        PROPERTIES
        LANGUAGE CXX
)
