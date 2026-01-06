configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)
add_definitions(-DHAVE_CONFIG_H)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(PROGRAM "echod")

set(SRCS
  base.h
  cfgfile.c
  cfgfile.h
  daemonize.c
  daemonize.h
  debug.h
  echod.h
  fnet.c
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
  report.c
  report.h
  security.c
  security.h
  sig2str.c
  sig2str.h
  ulog.c
  ulog.h
  valid.c
  valid.h
  xassert.c
  xassert.h
)

add_executable(${PROGRAM} ${SRCS})

if (NOT HAVE_GETOPT_LONG)
  message(STATUS "System getopt_long not found, using bundled implementation")

  target_sources(${PROGRAM} PRIVATE
      getopt.c
      getopt1.c
      getopt.h
  )

  target_compile_definitions(${PROGRAM} PRIVATE USE_BUNDLED_GETOPT)
else()
  message(STATUS "Using system getopt_long")
endif()

set_target_properties(${PROGRAM} PROPERTIES LINKER_LANGUAGE C)

install(TARGETS ${PROGRAM}
        RUNTIME DESTINATION ${CMAKE_INSTALL_SBINDIR}
)

add_executable(verbosity getopt.c getopt1.c verbosity.c)
add_executable(quiet getopt.c getopt1.c quiet.c)
add_executable(verbose getopt.c getopt1.c verbose.c)
