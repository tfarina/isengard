cmake_minimum_required(VERSION 2.6)
project(network C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

add_definitions(-D_GNU_SOURCE)

add_executable(hostname hostname.c)
