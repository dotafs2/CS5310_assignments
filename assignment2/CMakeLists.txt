cmake_minimum_required(VERSION 3.27)
project(assignment2)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
include_directories(.)

#add_executable(assignment2
#        Image.h
#        Image.c
#        fractals.h
#        fractals.cpp
#        mandeltest.cpp
#        )
#
#add_executable(assignment2
#        Image.h
#        Image.c
#        imagetest.c)


add_executable(assignment2
        Image.h
        Image.c
        fractals.h
        fractals.cpp
        perlinNoise.cpp
        perlinNoise.h
        main.cpp
)