configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)
add_definitions(-DHAVE_CONFIG_H)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(PROGRAM "echod")

set(SRCS
  assertion.c
  assertion.h
  cfgfile.c
  cfgfile.h
  daemonize.c
  daemonize.h
  debug.h
  echod.h
  fnet.c
  getopt.c
  getopt.h
  getopt1.c
  id.c
  id.h
  main.c
  optimization.h
  options.c
  options.h
  os_path.c
  os_path.h
  pathnames.h
  pidfile.c
  pidfile.h
  security.c
  security.h
  sig2str.c
  sig2str.h
  stringize.h
  ulog.c
  ulog.h
  valid.c
  valid.h
)

add_executable(${PROGRAM} ${SRCS})
set_target_properties(${PROGRAM} PROPERTIES LINKER_LANGUAGE C)

install(TARGETS ${PROGRAM}
        RUNTIME DESTINATION ${CMAKE_INSTALL_SBINDIR}
)

add_executable(verbosity getopt.c getopt1.c verbosity.c)
