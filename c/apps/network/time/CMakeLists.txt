cmake_minimum_required(VERSION 2.6)
project(time C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

add_definitions(-D_GNU_SOURCE)

set(TIMEC_SRCS log.c timec.c)
set(TIMED_SRCS log.c timed.c)

add_executable(timec ${TIMEC_SRCS})
add_executable(timed ${TIMED_SRCS})

install(TARGETS timec RUNTIME DESTINATION bin)
install(TARGETS timed RUNTIME DESTINATION bin)
