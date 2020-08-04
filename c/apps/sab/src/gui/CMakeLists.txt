find_package(GTK2 2.6 REQUIRED gtk)

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

add_executable(sab_gui
  ${SOURCES}
)

add_dependencies(sab_gui
  ${DEPENDENCIES}
)

target_include_directories(sab_gui PUBLIC
  ${GTK2_INCLUDE_DIRS}
)

target_link_libraries(sab_gui
  ${DEPENDENCIES}
  ${GTK2_LIBRARIES}
)
