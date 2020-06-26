cmake_minimum_required(VERSION 2.8)
project(vector C)

set(SRCS
    vector.c
    )

add_library(vector ${SRCS})

# Provide an option so the user can disable building tests.
option(BUILD_TESTS "Build the unit tests" ON)

if (BUILD_TESTS)
  add_executable(vector_test vector_test.c)
  target_link_libraries(vector_test vector)

  add_executable(unittests greatest.h unittests.c)
  target_link_libraries(unittests vector)
endif()
