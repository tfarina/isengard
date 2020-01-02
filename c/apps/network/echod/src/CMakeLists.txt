configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/ed_version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/ed_version.h"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(ECHOD_SRCS
  ed_cmdline.c
  ed_config.c
  ed_daemon.c
  ed_globals.c
  ed_log.c
  ed_main.c
  ed_net.c
  ed_path.c
  ed_pidfile.c
  ed_printf.c
  ed_privs.c
  ed_signals.c
  ed_utils.c
)

add_executable(echod ${ECHOD_SRCS})
set_target_properties(echod PROPERTIES LINKER_LANGUAGE C)

set(INSTALL_SBIN_DIR "${CMAKE_INSTALL_PREFIX}/sbin" CACHE PATH "Installation directory for system admin executables")

install(TARGETS echod RUNTIME DESTINATION "${INSTALL_SBIN_DIR}")
