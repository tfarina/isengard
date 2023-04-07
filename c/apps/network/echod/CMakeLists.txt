# Disallow in-source builds to prevent source tree corruption.
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR
    "In-source builds are not allowed! Make a separate folder for"
    " building.")
endif()

cmake_minimum_required(VERSION 3.0)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

project(echod
        VERSION "${VERSION}"
        LANGUAGES C)

set(PACKAGE           "${PROJECT_NAME}")
set(PACKAGE_NAME      "${PACKAGE}")
set(PACKAGE_TARNAME   "${PACKAGE}")
set(PACKAGE_VERSION   "${VERSION}")
set(PACKAGE_STRING    "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_BUGREPORT "")
set(PACKAGE_URL       "")

# Load local modules
list(APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/cmake/modules)

# User configurable options
option(ENABLE_NLS "Enable native language support" ON)

if (ENABLE_NLS)
  find_package(Intl REQUIRED)
endif()

include(DistTargets)
include(UninstallTarget)

configure_file(target.h.cmake.in target.h)

###############################################################################
# Provides install directory variables as defined by the GNU Coding Standards:
include(GNUInstallDirs)

set(prefix ${CMAKE_INSTALL_PREFIX})
set(localedir ${CMAKE_INSTALL_FULL_LOCALEDIR})

###############################################################################
include(CheckIncludeFile)
include(CheckFunctionExists)
include(CheckSymbolExists)

###############################################################################
# Check headers
check_include_file(getopt.h HAVE_GETOPT_H)
check_include_file(locale.h HAVE_LOCALE_H)

###############################################################################
# Check functions
check_function_exists(getopt_long HAVE_GETOPT_LONG)
if (HAVE_GETOPT_H)
  message("-- Found header getopt.h")
endif()

# Setup compiler flags
list(APPEND proposed_flags
  -ansi
  -std=c89
  -pedantic
  -Wall
  -Wextra
  -Werror
  -Wmissing-declarations
  -Wstrict-prototypes
  -Wmissing-prototypes
  -Wdeclaration-after-statement
)

foreach(flag IN LISTS proposed_flags)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}")
endforeach()

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
add_subdirectory(doc)

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
message(STATUS "Host Processor:    ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "Host OS:           ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "")
message(STATUS "Generator:         ${CMAKE_GENERATOR}")
message(STATUS "Build type:        ${CMAKE_BUILD_TYPE}")
message(STATUS "")
message(STATUS "Installation directories:")
message(STATUS "  prefix:           ${prefix}")
message(STATUS "  localedir:        ${localedir}")
message(STATUS "  sysconfdir:       ${SYSCONFDIR}")
message(STATUS "  localstatedir:    ${LOCALSTATEDIR}")
message(STATUS "")
message(STATUS "C compiler:        ${CMAKE_C_COMPILER}")
message(STATUS "C compiler flags:  ${CMAKE_C_FLAGS}")
message(STATUS "C linker flags:    ${CMAKE_C_LINK_FLAGS}")
message(STATUS "")
