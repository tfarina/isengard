cmake_minimum_required(VERSION 2.6)
project(sab C)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "1")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

option(WITH_GTK "Build GTK+ front end" OFF)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Place binaries for all build types in the same directory,
# so we know where to find it.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXECUTABLE_OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${EXECUTABLE_OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${EXECUTABLE_OUTPUT_PATH})

add_subdirectory(third_party/sqlite)
add_subdirectory(src)

add_compile_options(-std=c99)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Werror)
add_compile_options(-Wno-unused-parameter)

# This is a hack to allow the inclusion of "config.h".
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)

add_subdirectory(src/ui/cli)

if (WITH_GTK)
  add_subdirectory(src/ui/gtk)
endif()

message("BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")
