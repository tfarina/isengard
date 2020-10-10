configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

set(DEPENDENCIES
  sqlite3
)

set(SOURCES
  ab.c
  abook_contact.c
  alpm_list.c
  ffileutils.c
  futils.c
  mkdirp.c
  strutils.c
  test.c
  util.c
)

set(HEADERS
  ab.h
  abook_contact.h
  alpm_list.h
  arraysize.h
  ffileutils.h
  futils.h
  mkdirp.h
  strutils.h
  test.h
  util.h
)

add_library(ab
  ${SOURCES}
)

add_dependencies(ab
  ${DEPENDENCIES}
)

target_include_directories(ab PUBLIC
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(ab
  ${DEPENDENCIES}
)
