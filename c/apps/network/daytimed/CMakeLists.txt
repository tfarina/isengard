cmake_minimum_required(VERSION 2.6)
project(daytimed C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

add_definitions(-D_GNU_SOURCE)

set(DAYTIMED_SRCS fnet.c log.c daytimed.c)

add_executable(daytimed ${DAYTIMED_SRCS})

install(TARGETS daytimed RUNTIME DESTINATION bin)
