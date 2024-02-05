cmake_minimum_required(VERSION 2.6)
project(inih C)

set(SRCS
    ini.c
)

add_library(inih ${SRCS})
