cmake_minimum_required(VERSION 2.6)
project(echod C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall -Wextra")

#
# Output directories for a build tree
#
if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug")
endif()
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  set(CMAKE_C_FLAGS "-g -O2")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(CMAKE_C_FLAGS "-g -O0")
endif()

# Echod Version
set(ED_VERSION_MAJOR  0)
set(ED_VERSION_MINOR  0)
set(ED_VERSION_PATCH  1)
set(ED_VERSION_STR "${ED_VERSION_MAJOR}.${ED_VERSION_MINOR}.${ED_VERSION_PATCH}")

configure_file(
  "${PROJECT_SOURCE_DIR}/src/ed_version.h.in"
  "${PROJECT_SOURCE_DIR}/src/ed_version.h"
)

add_definitions(-D_GNU_SOURCE)

add_subdirectory(src)

set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")

install(FILES man/echod.8 DESTINATION "${INSTALL_MAN_DIR}/man8")

message(STATUS "")
message(STATUS "CMAKE_BUILD_TYPE:      ${CMAKE_BUILD_TYPE}")
message(STATUS "")
