cmake_minimum_required(VERSION 2.6)
project(echo C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

add_definitions(-D_GNU_SOURCE)

set(ECHOC_SRCS log.c echoc.c)
set(ECHOD_SRCS echod.c)

add_executable(echoc ${ECHOC_SRCS})
add_executable(echod ${ECHOD_SRCS})

install(TARGETS echoc RUNTIME DESTINATION sbin)
install(TARGETS echod RUNTIME DESTINATION sbin)
install(FILES echod.8 DESTINATION ${CMAKE_INSTALL_PREFIX}/man/man8)
