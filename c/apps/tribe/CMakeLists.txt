cmake_minimum_required(VERSION 2.6)
project(tribe C)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION_EXTRA "")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}${VERSION_EXTRA}")

set(PACKAGE           "${PROJECT_NAME}")

# Set the identity of this package.
set(PACKAGE_NAME "${PROJECT_NAME}")
set(PACKAGE_TARNAME "${PROJECT_NAME}")
set(PACKAGE_VERSION "${VERSION}")
set(PACKAGE_STRING "${PROJECT_NAME} ${VERSION}")
set(PACKAGE_BUGREPORT "")
set(PACKAGE_URL "")

configure_file(
  "config.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

# Load local modules
list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include(DistTargets)
include(UninstallTarget)

find_package(PkgConfig REQUIRED)

set(BINDIR "${CMAKE_INSTALL_PREFIX}/bin")
set(DATADIR "${CMAKE_INSTALL_PREFIX}/share")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Place binaries for all build types in the same directory,
# so we know where to find it.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXECUTABLE_OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${EXECUTABLE_OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${EXECUTABLE_OUTPUT_PATH})

list(APPEND c_flags
    -std=c99
    -Wall
    -Wextra
    -Werror
    -Wno-unused-parameter
)
foreach(flag IN LISTS c_flags)
    set(CMAKE_C_FLAGS "${flag} ${CMAKE_C_FLAGS}")
endforeach()

add_subdirectory(data)
add_subdirectory(src)
add_subdirectory(help)
