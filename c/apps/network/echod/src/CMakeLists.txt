configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)
add_definitions(-DHAVE_CONFIG_H)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/ed_version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/ed_version.h"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(PROGRAM "echod")

set(SRCS
  core.h
  daemon.c
  daemon.h
  debug.h
  echod.h
  ed_cmdline.c
  ed_net.c
  ed_privs.c
  main.c
  massert.c
  massert.h
  options.c
  options.h
  os_path.c
  os_path.h
  pidfile.c
  pidfile.h
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
