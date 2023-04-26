configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

add_subdirectory(third_party/sqlite)

set(DEPENDENCIES
  sqlite3
)

set(SOURCES
  ab.c
  ab_contact.c
  alpm_list.c
  dirs.c
  mkdirp.c
  os_path.c
  user.c
  util.c
  xstring.c
)

set(HEADERS
  ab.h
  ab_contact.h
  alpm_list.h
  arraysize.h
  mkdirp.h
  os_path.h
  user.h
  util.h
  xstring.h
)

add_library(ab
  ${SOURCES}
)

add_dependencies(ab
  ${DEPENDENCIES}
)

target_include_directories(ab PUBLIC
  ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(ab
  ${DEPENDENCIES}
)
