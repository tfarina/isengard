cmake_minimum_required(VERSION 2.8)
project(sbuf C)

set(SRCS
    sbuf.c
    )

add_library(sbuf ${SRCS})

add_executable(sbuf_test sbuf_test.c)
target_link_libraries(sbuf_test sbuf)
