set(DEPENDENCIES
  sqlite3
)

set(SOURCES
  ab.c
  abook_contact.c
  alpm_list.c
  strutils.c
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
