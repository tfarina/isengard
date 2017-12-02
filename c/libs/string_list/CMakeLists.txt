cmake_minimum_required(VERSION 2.8)
project(string_list C)

set(SRCS
    os_unix.c
    string_list.c
    )

add_library(string_list ${SRCS})
