cmake_minimum_required(VERSION 2.6)
project(echoc C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug")
endif()
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  set(CMAKE_C_FLAGS "-g -O2")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(CMAKE_C_FLAGS "-g -O0")
endif()

add_definitions(-D_GNU_SOURCE)

set(ECHOC_SRCS
  fnet.c
  main.c
  msg.c
  os_path.c
  validate.c
)

add_executable(echoc ${ECHOC_SRCS})

install(TARGETS echoc RUNTIME DESTINATION sbin)
