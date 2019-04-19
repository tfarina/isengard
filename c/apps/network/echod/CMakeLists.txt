cmake_minimum_required(VERSION 2.6)
project(echod C)

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

set(ECHOD_SRCS
  src/ed_config.c
  src/ed_daemon.c
  src/ed_instance.c
  src/ed_log.c
  src/ed_main.c
  src/ed_net.c
  src/ed_pid.c
  src/ed_printf.c
  src/ed_utils.c
)

add_executable(echod ${ECHOD_SRCS})

set(INSTALL_SBIN_DIR "${CMAKE_INSTALL_PREFIX}/sbin" CACHE PATH "Installation directory for system admin executables")
set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")

install(TARGETS echod RUNTIME DESTINATION "${INSTALL_SBIN_DIR}")
install(FILES man/echod.8 DESTINATION "${INSTALL_MAN_DIR}/man8")

message(STATUS "")
message(STATUS "CMAKE_BUILD_TYPE:      ${CMAKE_BUILD_TYPE}")
message(STATUS "")
