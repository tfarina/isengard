
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
#       DISABLE_TESTS
#           Build unit tests (default true). To disable the unit tests,
#           pass `-DDISABLE_TESTS=TRUE` during configuration.
#

cmake_minimum_required(VERSION 2.8)
project(vector C)

# Provide an option so the user can disable building tests.
option(DISABLE_TESTS "Do not build the unit tests (default is to build)" FALSE)
option(DISABLE_EXAMPLES "Do not build the example programs (default is to build)" FALSE)

set(SRCS
  vector.c
)

set(HDRS
  vector.h
)

add_library(vector ${SRCS})

if (NOT DISABLE_EXAMPLES)
  add_subdirectory(examples)
endif()

if (NOT DISABLE_TESTS)
  add_executable(unittests greatest.h vector_test.c unittests.c)
  target_link_libraries(unittests vector)
endif()
