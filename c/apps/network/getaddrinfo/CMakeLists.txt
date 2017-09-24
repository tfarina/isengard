cmake_minimum_required(VERSION 2.6)
project(getaddrinfo C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

add_definitions(-D_GNU_SOURCE)

set(SRCS getaddrinfo.c)

add_executable(getaddrinfo ${SRCS})

install(TARGETS getaddrinfo RUNTIME DESTINATION sbin)
