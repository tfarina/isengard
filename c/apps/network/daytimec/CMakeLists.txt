cmake_minimum_required(VERSION 2.6)
project(daytimec C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

add_definitions(-D_GNU_SOURCE)

set(DAYTIMEC_SRCS msg.c daytimec.c)

add_executable(daytimec ${DAYTIMEC_SRCS})

install(TARGETS daytimec RUNTIME DESTINATION bin)
