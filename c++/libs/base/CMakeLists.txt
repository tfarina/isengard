cmake_minimum_required(VERSION 2.8.12)

project(base CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(BASE_SOURCES
    directory.cc
    file_util.cc
    path.cc
    )

add_library(base ${BASE_SOURCES})
