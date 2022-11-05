cmake_minimum_required(VERSION 2.6)
project(httpd C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

add_definitions(-D_GNU_SOURCE)

set(SRCS msg.c fnet.c httpd.c)

add_executable(httpd ${SRCS})

install(TARGETS httpd RUNTIME DESTINATION sbin)
