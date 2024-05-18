cmake_minimum_required(VERSION 3.27)
project(assignment2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(assignment2
        imagetest.c
        Image.c
        Image.h)
