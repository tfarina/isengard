cmake_minimum_required(VERSION 2.6)
project(sqlite3 C)

set(SRCS amalgamation/sqlite3.c)

add_library(sqlite3 ${SRCS})
target_link_libraries(sqlite3 -pthread ${CMAKE_DL_LIBS})
