add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/sqlite third_party/sqlite)

set(TARGETNAME ab)

set(DEPENDENCIES
  sqlite
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

add_library(${TARGETNAME}
  ${SOURCES}
)

add_dependencies(${TARGETNAME}
  ${DEPENDENCIES}
)

target_include_directories(${TARGETNAME} PUBLIC
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${TARGETNAME}
  ${DEPENDENCIES}
)

add_subdirectory(ui)
