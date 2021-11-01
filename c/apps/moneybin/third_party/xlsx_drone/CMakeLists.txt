cmake_minimum_required(VERSION 2.6)
project(xlsx_drone C)

set(HDRS
    xlsx_drone.h
    sxmlsearch.h
    sxmlc.h
    miniz.h
    zip.h)

set(SRCS
    xlsx_drone.c
    sxmlsearch.c
    sxmlc.c
    zip.c)

add_library(xlsx_drone ${HDRS} ${SRCS})
