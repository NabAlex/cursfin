cmake_minimum_required(VERSION 3.6)
project(cursfin)

find_package(PkgConfig)

set(CMAKE_CXX_STANDARD 11)

include_directories(LIB "include")
pkg_check_modules(GTKMM gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -s -O3")

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
        include/base_drawer.h

        include/perlin_noise.h

        include/texture.h
        src/texture.cpp

        include/model.h
        src/model.cpp

        include/light.h
        src/light.cpp

        include/scalar_field.h
        src/scalar_field.cpp

        include/object.h)

add_executable(cursfin ${SOURCE_FILES})
target_link_libraries(cursfin
        ${GTKMM_LIBRARIES})