# set the minimum required version of CMake 
cmake_minimum_required(VERSION 3.10)

# check the supported platform
if(NOT UNIX)
    message(FATAL_ERROR "Only *nix like systems are supported")
endif()

# set the project name
project(utilbox)

# set the project version
set(UTILBOX_MAJOR_VERSION 0)
set(UTILBOX_MINOR_VERSION 1)
set(UTILBOX_MICRO_VERSION 1)
set(UTILBOX_VERSION ${UTILBOX_MAJOR_VERSION}.${UTILBOX_MINOR_VERSION}.${UTILBOX_MICRO_VERSION})
set(UTILBOX_SOVERSION ${UTILBOX_MAJOR_VERSION}.${UTILBOX_MINOR_VERSION})

# include custom modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMakeModules")
message(CHECK_START  "${CMAKE_MODULE_PATH}")


# add the source code directory
add_subdirectory(source/conf)
add_subdirectory(source)
