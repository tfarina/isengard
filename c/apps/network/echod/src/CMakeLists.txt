configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)
add_definitions(-DHAVE_CONFIG_H)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(PROGRAM "echod")

set(SRCS
  assertion.c
  assertion.h
  daemonize.c
  daemonize.h
  debug.h
  echod.h
  ed_net.c
  getopt.c
  getopt.h
  getopt1.c
  main.c
  optimization.h
  options.c
  options.h
  os_path.c
  os_path.h
  pidfile.c
  pidfile.h
  security.c
  security.h
  sig2str.c
  sig2str.h
  stringize.h
  ulog.c
  ulog.h
  username.c
  username.h
  valid.c
  valid.h
)

add_executable(${PROGRAM} ${SRCS})
set_target_properties(${PROGRAM} PROPERTIES LINKER_LANGUAGE C)

install(TARGETS ${PROGRAM}
        RUNTIME DESTINATION ${CMAKE_INSTALL_SBINDIR}
)
