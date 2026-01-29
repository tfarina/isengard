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
  xalloc.c
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
  xalloc.h
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

add_subdirectory(cli)

#
# Tribe binary
#
set(TARGETNAME "${PROJECT_NAME}")

set(DEPENDENCIES
  ab
  sqlite
)

set(SOURCES
  about_dialog.c
  contact_editor.c
  main.c
  window.c
)

add_executable(${TARGETNAME}
  ${SOURCES}
)

add_dependencies(${TARGETNAME}
  ${DEPENDENCIES}
)

target_compile_options(${TARGETNAME} PUBLIC
  ${GTK_CFLAGS}
)

target_include_directories(${TARGETNAME} PUBLIC
  ${CMAKE_BINARY_DIR}
  ${GTK_INCLUDE_DIRS}
)

target_link_libraries(${TARGETNAME}
  ${DEPENDENCIES}
  ${GTK_LDFLAGS}
)

install(TARGETS ${TARGETNAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
