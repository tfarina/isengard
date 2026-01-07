cmake_minimum_required(VERSION 2.8)
project(buffer C)

set(SRCS
    buffer.c
    )

add_library(buffer ${SRCS})

add_executable(buffer_example buffer_example.c)
target_link_libraries(buffer_example buffer)

add_executable(buffer_unittests utest.h buffer_test.c run_all_unittests.c)
target_link_libraries(buffer_unittests buffer)
