cmake_minimum_required(VERSION 2.6)
project(sqlite3 C)

set(TARGETNAME sqlite3)

set(SOURCES
  amalgamation/sqlite3.c
)

add_library(${TARGETNAME} ${SOURCES})
target_link_libraries(${TARGETNAME} -pthread ${CMAKE_DL_LIBS})
