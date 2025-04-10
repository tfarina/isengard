cmake_minimum_required(VERSION 2.6)
project(tribe C)

# **************************
# Define version information
# **************************
# For a release candidate, set VERSION_EXTRA to -rc1, -rc2, and so on.
# Example: set(VERSION_EXTRA "-rc1")
# For a release, leave VERSION_EXTRA empty.
# Example: set(VERSION_EXTRA "")
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
  "config.h.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)
add_definitions(-DHAVE_CONFIG_H)

# Load local modules
list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include(GNUInstallDirs) # for CMAKE_INSTALL_<dir> variables
include(DistTargets)
include(UninstallTarget)

# Place binaries for all build types in the same directory,
# so we know where to find it.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR} CACHE PATH "Output directory for runtime binaries")

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

# ***************************
# Check for required programs
# ***************************
find_package(PkgConfig REQUIRED)

# *************************
# Minimum required versions
# *************************
set(glib_min_version 2.19.7)
set(gtk_min_version 2.16.1)

# ***************************
# Check for required packages
# ***************************
pkg_check_modules(GLIB REQUIRED
  glib-2.0>=${glib_min_version}
)
pkg_check_modules(GTK REQUIRED
  gtk+-2.0>=${gtk_min_version}
)

add_subdirectory(data)
add_subdirectory(src)
add_subdirectory(help)
