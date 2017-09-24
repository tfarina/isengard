cmake_minimum_required(VERSION 2.6)
project(showip C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

add_definitions(-D_GNU_SOURCE)

set(SRCS showip.c)

add_executable(showip ${SRCS})

install(TARGETS showip RUNTIME DESTINATION sbin)
