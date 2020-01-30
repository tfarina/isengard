if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Do not build in-source.\nPlease remove CMakeCache.txt and the CMakeFiles/ directory.\nThen: mkdir build ; cd build ; then call cmake from there.")
endif()

cmake_minimum_required(VERSION 2.6)
project(echod C)

include(CheckIncludeFiles)

check_include_files(getopt.h HAVE_GETOPT_H)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-ansi -std=c89 -pedantic -Wall -Wextra -Werror -Wstrict-prototypes -Wno-unused-parameter")

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
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
endif()

# Echod Version
set(ED_VERSION_MAJOR  0)
set(ED_VERSION_MINOR  1)
set(ED_VERSION_PATCH  0)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  add_definitions(-D_GNU_SOURCE)
endif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")

if (DEFINED CMAKE_INSTALL_PREFIX)
  set(SYSCONFDIR ${CMAKE_INSTALL_PREFIX}/etc CACHE PATH "configuration directory")
  set(LOCALSTATEDIR ${CMAKE_INSTALL_PREFIX}/var CACHE PATH "var directory")
endif()

if (DEFINED SYSCONFDIR)
  set(SYSCONFDIR ${SYSCONFDIR} CACHE PATH "configuration directory")
else()
  set(SYSCONFDIR ${CMAKE_INSTALL_PREFIX}/etc CACHE PATH "configuration directory")
endif()

if (DEFINED LOCALSTATEDIR)
  set(LOCALSTATEDIR ${LOCALSTATEDIR} CACHE PATH "var directory")
else()
  set(LOCALSTATEDIR ${CMAKE_INSTALL_PREFIX}/var CACHE PATH "var directory")
endif()


add_definitions("-DSYSCONFDIR=\"${SYSCONFDIR}\"")
add_definitions("-DLOCALSTATEDIR=\"${LOCALSTATEDIR}\"")

add_subdirectory(src)

set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")

install(FILES doc/man/man8/echod.8 DESTINATION "${INSTALL_MAN_DIR}/man8" COMPONENT doc)

message(STATUS "Running cmake version ${CMAKE_VERSION}")
message(STATUS "")
message(STATUS "Host Processor: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "Host OS: ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "")
message(STATUS "Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "sysconfdir:     ${SYSCONFDIR}")
message(STATUS "localstatedir:  ${LOCALSTATEDIR}")
message(STATUS "")
message(STATUS "CMAKE_BUILD_TYPE:      ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_C_FLAGS:         ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_C_LINK_FLAGS:    ${CMAKE_C_LINK_FLAGS}")
message(STATUS "")
