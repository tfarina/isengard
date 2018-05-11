cmake_minimum_required(VERSION 2.8)
project(string_array C)

set(SRCS
    log.c
    xalloc.c
    string_array.c
    )

add_library(string_array ${SRCS})

add_executable(string_array_test string_array_test.c)
target_link_libraries(string_array_test string_array)
