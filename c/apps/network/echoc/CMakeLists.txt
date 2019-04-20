cmake_minimum_required(VERSION 2.6)
project(echoc C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

add_definitions(-D_GNU_SOURCE)

set(ECHOC_SRCS
  ec_main.c
  ec_net.c
  ec_utils.c
  log.c
)

add_executable(echoc ${ECHOC_SRCS})

install(TARGETS echoc RUNTIME DESTINATION sbin)
