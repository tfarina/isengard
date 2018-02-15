cmake_minimum_required(VERSION 2.8)
project(trade C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(LIBCSV_SRCS
    libcsv.c
    )

add_library(libcsv ${LIBCSV_SRCS})

add_executable(ticks ticks.c)
target_link_libraries(ticks libcsv)
