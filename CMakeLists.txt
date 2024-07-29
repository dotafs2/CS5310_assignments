cmake_minimum_required(VERSION 3.27)
project(assignment5 C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

# Enable verbose makefile output for debugging
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set the my windows and mac have different path
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
 set(CMAKE_PREFIX_PATH "C:/Qt/6.7.1/mingw_64")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
 set(CMAKE_PREFIX_PATH "/Users/dotafs/Qt/6.7.2/macos")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(include)

# Define common sources and headers
set(COMMON_SOURCE_FILES
        src/Image.c
        src/Line.c
        include/Line.h
        src/anti-alias.cpp
        src/ball.cpp
        src/Polygon.c
        include/Polygon.h
        src/fsMath.c
        include/fsMath.h
        include/Module.h
        src/Module.c
        src/Bezier.c
        include/Bezier.h
        src/Light.c
        include/Light.h
        src/plyRead.c
        include/plyRead.h
)

# QT
set(MAIN_SOURCE_FILES
        ${COMMON_SOURCE_FILES}
        include/mainwindow.h
        src/mainwindow.cpp
        src/main.cpp
        include/WaterSimulation.h
        src/WaterSimulation.cpp
        test/test.cpp
        test/test.h
)

# test6a
set(TEST6A_SOURCE_FILES
        ${COMMON_SOURCE_FILES}
        test/test7/test7a.c
)

# test6b
set(TEST6B_SOURCE_FILES
        ${COMMON_SOURCE_FILES}
        test/test6/test6b.c
)

# Create the main entry executable
add_executable(main_entry ${MAIN_SOURCE_FILES})

set_source_files_properties(
        src/anti-alias.cpp
        src/ball.cpp
        src/main.cpp
        src/mainwindow.cpp
        src/WaterSimulation.cpp
        PROPERTIES
        LANGUAGE CXX
)

# Link Qt6 Widgets library for main entry
target_link_libraries(main_entry Qt6::Core Qt6::Widgets)

# Create the test6a executable
add_executable(test6a ${TEST6A_SOURCE_FILES})

set_source_files_properties(
        src/anti-alias.cpp
        src/ball.cpp
        src/WaterSimulation.cpp
        PROPERTIES
        LANGUAGE CXX
)

# Link Qt6 Widgets library for test6a
target_link_libraries(test6a Qt6::Core Qt6::Widgets)

# Create the test6b executable
add_executable(test6b ${TEST6B_SOURCE_FILES})

set_source_files_properties(
        src/anti-alias.cpp
        src/ball.cpp
        src/WaterSimulation.cpp
        PROPERTIES
        LANGUAGE CXX
)

# Link Qt6 Widgets library for test6b
target_link_libraries(test6b Qt6::Core Qt6::Widgets)