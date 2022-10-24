# Disallow in-source builds to prevent source tree corruption.
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR
    "In-source builds are not allowed! Make a separate folder for"
    " building.")
endif()

cmake_minimum_required(VERSION 3.0)
project(echod LANGUAGES C VERSION 0.1.0)

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

# Add local cmake modules
list(APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/cmake/modules)

# User configurable options
option(ENABLE_NLS "Enable native language support" ON)

if (ENABLE_NLS)
  find_package(Intl REQUIRED)
endif()

include(UninstallTarget)

#=======================================================================================
# Provides GNU standard installation directories as defined by the GNU Coding Standards:
#=======================================================================================
include(GNUInstallDirs)

include(CheckIncludeFile)
include(CheckFunctionExists)
include(CheckSymbolExists)

check_include_file(getopt.h HAVE_GETOPT_H)
check_function_exists(getopt_long HAVE_GETOPT_LONG)
if (HAVE_GETOPT_H)
  message("-- Found header getopt.h")
endif()

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

# Check if _GNU_SOURCE is available.

if (NOT _GNU_SOURCE)
  check_symbol_exists(__GNU_LIBRARY__ "features.h" _GNU_SOURCE)

  if (NOT _GNU_SOURCE)
    unset(_GNU_SOURCE CACHE)
    check_symbol_exists(_GNU_SOURCE "features.h" _GNU_SOURCE)
  endif()
endif()

if (_GNU_SOURCE)
  add_definitions(-D_GNU_SOURCE)
endif()

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

## ==============================================================================
##
##  Configuration summary
##
## ==============================================================================

message(STATUS "------------------------------------------------------------" )
message(STATUS "[Echod] Configuration summary."                               )
message(STATUS "------------------------------------------------------------ ")
message(STATUS "Running cmake version ${CMAKE_VERSION}")
message(STATUS "")
message(STATUS "Generator:         ${CMAKE_GENERATOR}")
message(STATUS "")
message(STATUS "Host Processor:    ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "Host OS:           ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "")
message(STATUS "Build type:        ${CMAKE_BUILD_TYPE}")
message(STATUS "")
message(STATUS "Installation directories:")
message(STATUS "  prefix:           ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  sysconfdir:       ${SYSCONFDIR}")
message(STATUS "  localstatedir:    ${LOCALSTATEDIR}")
message(STATUS "")
message(STATUS "CMAKE_C_COMPILER_ID: ${CMAKE_C_COMPILER_ID}")
message(STATUS "CMAKE_C_COMPILER:   ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_C_FLAGS:      ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_C_LINK_FLAGS: ${CMAKE_C_LINK_FLAGS}")
message(STATUS "")
