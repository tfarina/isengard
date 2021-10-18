cmake_minimum_required(VERSION 2.6)
project(zip C)

set(SRCS
    zip.c)

add_library(zip ${SRCS})
