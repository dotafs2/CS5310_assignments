cmake_minimum_required(VERSION 3.27)
project(assignment4 C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
set(Qt6_DIR "/opt/homebrew/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories(include)

set(SOURCE_FILES
        src/Image.c
        src/Line.c
        src/anti-alias.cpp
        src/ball.cpp
        src/Polygon.c
        include/Polygon.h
        #src/main.cpp
       # include/mainwindow.h
        #src/mainwindow.cpp
       # test/test4a.c
        test/polyspeed.c
)

add_executable(assignment4 ${SOURCE_FILES})

set_source_files_properties(
        src/anti-alias.cpp
        src/ball.cpp
        #src/main.cpp
      #  src/mainwindow.cpp
        PROPERTIES
        LANGUAGE CXX
)

# qt6_wrap_cpp(MOC_SOURCES include/mainwindow.h)

target_sources(assignment4 PRIVATE ${MOC_SOURCES})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets)
