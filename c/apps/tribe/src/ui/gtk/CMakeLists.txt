set(GTK_REQUIRED 2.10.0)

# Check for required packages
pkg_check_modules(GTK2 REQUIRED
  gtk+-2.0>=${GTK_REQUIRED}
)

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

target_include_directories(${TARGETNAME} PUBLIC
  ${CMAKE_BINARY_DIR}
  ${GTK2_INCLUDE_DIRS}
)

target_link_libraries(${TARGETNAME}
  ${DEPENDENCIES}
  ${GTK2_LIBRARIES}
)

install(TARGETS ${TARGETNAME}
  DESTINATION ${BINDIR}
)
