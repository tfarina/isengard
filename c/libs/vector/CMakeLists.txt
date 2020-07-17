
# Description
# ===========
#
#   Use:
#       Move to a custom directory, ideally out of source.
#
#   Example:
#       For example, in the project directory, to build library
#       but not the unit tests in debug mode, type:
#           mkdir build && cd build
#           cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=NO ..
#
#   Flags:
#       ENABLE_TESTS
#           Build unit tests (default true). To disable the unit tests,
#           pass `-DENABLE_TESTS=NO` during configuration.
#

cmake_minimum_required(VERSION 2.8)
project(vector C)

include(GNUInstallDirs)

# Provide an option so the user can disable building tests.
option(ENABLE_TESTS "Build the unit tests (default is to build)" YES)
option(ENABLE_EXAMPLES "Build the example programs (default is to build)" YES)

set(SRCS
  vector.c
)

set(HDRS
  vector.h
)

add_library(vector ${SRCS})

if (ENABLE_EXAMPLES)
  add_subdirectory(examples)
endif()

if (ENABLE_TESTS)
  add_subdirectory(tests)
endif()

configure_file(libvector.pc.in libvector.pc)
