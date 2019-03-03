cmake_minimum_required(VERSION 2.6)
project(iniparser C)

set(SRCS
    iniparser.c
    dictionary.c)

add_library(iniparser ${SRCS})
