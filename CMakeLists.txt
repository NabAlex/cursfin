cmake_minimum_required(VERSION 3.6)
project(cursfin)

find_package(PkgConfig)

set(CMAKE_CXX_STANDARD 11)

include_directories(LIB "include")
pkg_check_modules(GTKMM gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp
        include/draw.h
        src/draw.cpp
        include/camera.h
        src/camera.cpp
        include/util.h
        src/util.cpp)

add_executable(cursfin ${SOURCE_FILES})
target_link_libraries(cursfin
        ${GTKMM_LIBRARIES})