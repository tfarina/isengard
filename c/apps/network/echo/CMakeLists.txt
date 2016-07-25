cmake_minimum_required(VERSION 2.6)
project(echo C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

add_definitions(-D_GNU_SOURCE)

set(PROGRAM_SOURCES echod.c)

add_executable(echod ${PROGRAM_SOURCES})

install(TARGETS echod RUNTIME DESTINATION bin)
