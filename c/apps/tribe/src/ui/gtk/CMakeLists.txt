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
