cmake_minimum_required(VERSION 3.27)
project(assignment3 C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(assignment3
        include/Image.h
#        src/circleMidpoint.c
#        src/ellipseMidpoint.c
        src/Image.c
        # src/testbench.c
        src/Line.c
        include/Line.h
        src/test3a.c)


        #src/test3b.c
       # src/test3d.c)
