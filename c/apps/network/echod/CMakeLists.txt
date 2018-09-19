cmake_minimum_required(VERSION 2.6)
project(echod C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall")

add_definitions(-D_GNU_SOURCE)

set(ECHOD_SRCS src/ed_log.c src/echod.c)

add_executable(echod ${ECHOD_SRCS})

set(INSTALL_SBIN_DIR "${CMAKE_INSTALL_PREFIX}/sbin" CACHE PATH "Installation directory for system admin executables")
set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")

install(TARGETS echod RUNTIME DESTINATION "${INSTALL_SBIN_DIR}")
install(FILES man/echod.8 DESTINATION "${INSTALL_MAN_DIR}/man8")
