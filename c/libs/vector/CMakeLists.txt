
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
#           cmake -GNinja -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Debug ..
#
#   Flags:
#       BUILD_TESTS
#           Build unit tests (default on). To disable the unit tests,
#           pass `-DBUILD_TESTS=OFF` during configuration.
#

cmake_minimum_required(VERSION 2.8)
project(vector C)

# Provide an option so the user can disable building tests.
option(BUILD_TESTS "Build the unit tests" ON)
option(BUILD_EXAMPLES "Build the example programs" ON)

set(SRCS
  vector.c
)

set(HDRS
  vector.h
)

add_library(vector ${SRCS})

if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if (BUILD_TESTS)
  add_executable(unittests greatest.h vector_test.c unittests.c)
  target_link_libraries(unittests vector)
endif()
