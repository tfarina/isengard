cmake_minimum_required(VERSION 2.6)
project(echo C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

add_definitions(-D_GNU_SOURCE)

set(ECHOC_SRCS die.c echoc.c)
set(ECHOD_SRCS echod.c)

add_executable(echoc ${ECHOC_SRCS})
add_executable(echod ${ECHOD_SRCS})

install(TARGETS echoc RUNTIME DESTINATION bin)
install(TARGETS echod RUNTIME DESTINATION bin)
