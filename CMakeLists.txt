cmake_minimum_required(VERSION 3.6)
project(cursfin)

find_package(PkgConfig)

set(CMAKE_CXX_STANDARD 11)

include_directories(LIB "include")
pkg_check_modules(GTKMM gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp
        include/scen_controller.h
        src/scen_controller.cpp
        include/camera.h
        src/camera.cpp
        include/util.h
        src/util.cpp
        include/creator.h
        src/creator.cpp
        include/easy_scen_controller.h
        src/easy_scen_controller.cpp
        include/renderer.h
        src/renderer.cpp
        include/base_drawer.h)

add_executable(cursfin ${SOURCE_FILES})
target_link_libraries(cursfin
        ${GTKMM_LIBRARIES})