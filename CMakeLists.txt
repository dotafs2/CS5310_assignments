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
        src/Particle.c
        include/Particle.h
        src/plyRead.c
        include/plyRead.h
)

# QT_realtime
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

# QT_offline
set(OFFLINE_SOURCE_FILES
        ${COMMON_SOURCE_FILES}
        include/mainWindowOffline.h
        src/mainWindowOffline.cpp
        src/mainOffline.cpp
        include/WaterSimulation.h
        src/WaterSimulation.cpp
        src/perlinNoise.cpp
        include/perlinNoise.h
)

# test6a
set(TEST6A_SOURCE_FILES
        ${COMMON_SOURCE_FILES}
        test/test9/test9a.c
)

# test6b
set(TEST6B_SOURCE_FILES
        ${COMMON_SOURCE_FILES}
        test/test9/test9c.c
)

# Create the main entry executable
add_executable(main_entry ${MAIN_SOURCE_FILES})
# Create the offline entry executable
add_executable(offline_entry ${OFFLINE_SOURCE_FILES})


# Link Qt6 Widgets library for main entry
target_link_libraries(main_entry Qt6::Core Qt6::Widgets)
# Link Qt6 Widgets library for offline entry
target_link_libraries(offline_entry Qt6::Core Qt6::Widgets)

# Main entry
#set_source_files_properties(
#        src/anti-alias.cpp
#        src/ball.cpp
#        src/main.cpp
#        src/WaterSimulation.cpp
#        PROPERTIES
#        LANGUAGE CXX
#)

## Offline entry
#set_source_files_properties(
#        src/anti-alias.cpp
#        src/ball.cpp
#        src/main.cpp
#        src/WaterSimulation.cpp
#        PROPERTIES
#        LANGUAGE CXX
#)


# Create the test6a executable
add_executable(test6a ${TEST6A_SOURCE_FILES})

# Create the test6b executable
add_executable(test6b ${TEST6B_SOURCE_FILES})

# Link Qt6 Widgets library for test6a
target_link_libraries(test6a Qt6::Core Qt6::Widgets)

# Link Qt6 Widgets library for test6b
target_link_libraries(test6b Qt6::Core Qt6::Widgets)

#
#set_source_files_properties(
#        src/anti-alias.cpp
#        src/ball.cpp
#        src/WaterSimulation.cpp
#        PROPERTIES
#        LANGUAGE CXX
#)
