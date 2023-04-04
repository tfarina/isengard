# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)

set(EXENAME "${PROJECT_NAME}_gui")

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

add_executable(${EXENAME}
  ${SOURCES}
)

add_dependencies(${EXENAME}
  ${DEPENDENCIES}
)

target_include_directories(${EXENAME} PUBLIC
  ${GTK2_INCLUDE_DIRS}
)

target_link_libraries(${EXENAME}
  ${DEPENDENCIES}
  ${GTK2_LIBRARIES}
)

install(TARGETS ${EXENAME}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
