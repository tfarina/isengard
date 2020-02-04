configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/ed_version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/ed_version.h"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(PROGRAM "echod")

set(SRCS
  daemon.c
  daemon.h
  debug.h
  ed_cmdline.c
  ed_globals.c
  ed_net.c
  ed_pidfile.c
  ed_privs.c
  main.c
  option.c
  option.h
  os_path.c
  os_path.h
  sig2str.c
  sig2str.h
  ulog.c
  ulog.h
  username.c
  username.h
  valid.c
  valid.h
)

add_executable(${PROGRAM} ${SRCS})
set_target_properties(${PROGRAM} PROPERTIES LINKER_LANGUAGE C)

set(INSTALL_SBIN_DIR "${CMAKE_INSTALL_PREFIX}/sbin" CACHE PATH "Installation directory for system admin executables")

install(TARGETS ${PROGRAM} RUNTIME DESTINATION "${INSTALL_SBIN_DIR}")
