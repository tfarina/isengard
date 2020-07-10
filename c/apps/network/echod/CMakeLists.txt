
# Disallow in-source builds to prevent source tree corruption.
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR
    "In-source builds are not allowed! Make a separate folder for"
    " building.")
endif()

cmake_minimum_required(VERSION 3.0)
project(echod LANGUAGES C VERSION 0.1.0)

# Add local cmake modules
list(APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/cmake/modules)

include(UninstallTarget)

###############################################################################
# Provides install directory variables as defined by the GNU Coding Standards.
include(GNUInstallDirs)

include(CheckIncludeFiles)
include(CheckFunctionExists)

check_include_files(getopt.h HAVE_GETOPT_H)

check_function_exists(getopt HAVE_GETOPT)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-ansi -std=c89 -pedantic -Wall -Wextra -Werror -Wmissing-declarations -Wstrict-prototypes -Wmissing-prototypes -Wdeclaration-after-statement")

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

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  add_definitions(-D_GNU_SOURCE)
endif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")

if (DEFINED CMAKE_INSTALL_PREFIX)
  set(SYSCONFDIR ${CMAKE_INSTALL_PREFIX}/etc CACHE PATH "configuration directory")
  set(LOCALSTATEDIR ${CMAKE_INSTALL_PREFIX}/var CACHE PATH "var directory")
endif()

# If a sysconfdir is specified, use it instead of the default configuration directory.
# e.g., cmake -DSYSCONFDIR=/etc
if (DEFINED SYSCONFDIR)
  set(SYSCONFDIR ${SYSCONFDIR} CACHE PATH "configuration directory")
else()
  set(SYSCONFDIR ${CMAKE_INSTALL_PREFIX}/etc CACHE PATH "configuration directory")
endif()

# If a localstatedir is specified, use it instead of the default var directory.
# e.g., cmake -DLOCALSTATEDIR=/var
if (DEFINED LOCALSTATEDIR)
  set(LOCALSTATEDIR ${LOCALSTATEDIR} CACHE PATH "var directory")
else()
  set(LOCALSTATEDIR ${CMAKE_INSTALL_PREFIX}/var CACHE PATH "var directory")
endif()

add_definitions("-DSYSCONFDIR=\"${SYSCONFDIR}\"")
add_definitions("-DLOCALSTATEDIR=\"${LOCALSTATEDIR}\"")

add_subdirectory(src)

install(FILES doc/man/man8/echod.8 DESTINATION "${CMAKE_INSTALL_MANDIR}/man8" COMPONENT doc)

message(STATUS "Running cmake version ${CMAKE_VERSION}")
message(STATUS "")
message(STATUS "Host Processor:    ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "Host OS:           ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "")
message(STATUS "Install prefix:     ${CMAKE_INSTALL_PREFIX}")
message(STATUS "sysconfdir:         ${SYSCONFDIR}")
message(STATUS "localstatedir:      ${LOCALSTATEDIR}")
message(STATUS "")
message(STATUS "CMAKE_BUILD_TYPE:   ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_C_COMPILER:   ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_C_FLAGS:      ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_C_LINK_FLAGS: ${CMAKE_C_LINK_FLAGS}")
message(STATUS "")
