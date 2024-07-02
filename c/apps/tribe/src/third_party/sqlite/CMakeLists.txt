cmake_minimum_required(VERSION 2.6)
project(sqlite3 C)

set(TARGETNAME sqlite3)

add_compile_options(-std=c99)
add_compile_options(-pedantic)
add_compile_options(-pthread)

set(SRCS amalgamation/sqlite3.c)

add_library(${TARGETNAME} ${SRCS})
target_link_libraries(${TARGETNAME} -pthread ${CMAKE_DL_LIBS})
