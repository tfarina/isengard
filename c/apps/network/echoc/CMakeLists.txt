cmake_minimum_required(VERSION 2.6)
project(echoc C)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(PACKAGE           "${PROJECT_NAME}")
set(PACKAGE_BUGREPORT "")
set(PACKAGE_NAME      "${PACKAGE}")
set(PACKAGE_TARNAME   "${PACKAGE}")
set(PACKAGE_URL       "")
set(PACKAGE_VERSION   "${VERSION}")
set(PACKAGE_STRING    "${PACKAGE_NAME} ${PACKAGE_VERSION}")

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug")
endif()
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  set(CMAKE_C_FLAGS "-g -O2")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(CMAKE_C_FLAGS "-g -O0")
endif()

add_definitions(-D_GNU_SOURCE)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

# This is a hack to allow the inclusion of "config.h".
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(ECHOC_SRCS
  src/check.c
  src/fnet.c
  src/getopt.c
  src/getopt1.c
  src/main.c
  src/msg.c
  src/options.c
  src/os_path.c
)

add_executable(echoc ${ECHOC_SRCS})

install(TARGETS echoc RUNTIME DESTINATION ${CMAKE_INSTALL_SBINDIR})
