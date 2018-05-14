cmake_minimum_required(VERSION 2.8)
project(buffer C)

set(SRCS
    buffer.c
    )

add_library(buffer ${SRCS})

add_executable(buffer_test buffer_test.c)
target_link_libraries(buffer_test buffer)
