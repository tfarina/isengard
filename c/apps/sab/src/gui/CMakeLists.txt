find_package(GTK2 2.6 REQUIRED gtk)

set(EXE_NAME "sab_gui")

set(DEPENDENCIES
  ab
  sqlite3
)

set(SOURCES
  about_dialog.c
  contact_editor.c
  main.c
  window.c
)

add_executable(${EXE_NAME}
  ${SOURCES}
)

add_dependencies(${EXE_NAME}
  ${DEPENDENCIES}
)

target_include_directories(${EXE_NAME} PUBLIC
  ${GTK2_INCLUDE_DIRS}
)

target_link_libraries(${EXE_NAME}
  ${DEPENDENCIES}
  ${GTK2_LIBRARIES}
)
