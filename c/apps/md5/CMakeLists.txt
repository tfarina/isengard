cmake_minimum_required(VERSION 2.6)
project(md5 C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

add_definitions(-D_GNU_SOURCE)

set(MD5_SRCS
  main.c
  md5.c)

add_executable(md5 ${MD5_SRCS})

set(INSTALL_SBIN_DIR "${CMAKE_INSTALL_PREFIX}/sbin" CACHE PATH "Installation directory for system admin executables")
set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")

install(TARGETS md5 RUNTIME DESTINATION "${INSTALL_SBIN_DIR}")
install(FILES man/md5.8 DESTINATION "${INSTALL_MAN_DIR}/man8")
