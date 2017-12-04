cmake_minimum_required(VERSION 2.8)
project(string_list C)

set(SRCS
    log.c
    os_unix.c
    string_list.c
    )

add_library(string_list ${SRCS})

add_executable(string_list_test string_list_test.c)
target_link_libraries(string_list_test string_list)
