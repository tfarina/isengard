cmake_minimum_required(VERSION 2.6)
project(echod C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

add_definitions(-D_GNU_SOURCE)

set(ECHOD_SRCS src/echod.c)

add_executable(echod ${ECHOD_SRCS})

install(TARGETS echod RUNTIME DESTINATION sbin)
install(FILES src/echod.8 DESTINATION ${CMAKE_INSTALL_PREFIX}/man/man8)
