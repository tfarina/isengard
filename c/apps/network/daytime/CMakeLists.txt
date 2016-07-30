cmake_minimum_required(VERSION 2.6)
project(time C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

add_definitions(-D_GNU_SOURCE)

set(DAYTIMEC_SRCS log.c daytimec.c)
set(DAYTIMED_SRCS log.c daytimed.c)

add_executable(daytimec ${DAYTIMEC_SRCS})
add_executable(daytimed ${DAYTIMED_SRCS})

install(TARGETS daytimec RUNTIME DESTINATION bin)
install(TARGETS daytimed RUNTIME DESTINATION bin)
