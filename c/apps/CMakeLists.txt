cmake_minimum_required(VERSION 2.6)
project(iniparser C)

set(SRCS
    dictionary.c
    iniparser.c
    )

add_library(iniparser ${SRCS})
