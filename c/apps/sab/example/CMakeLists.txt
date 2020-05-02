find_package(GTK2 2.6 REQUIRED gtk)

set(DEPENDENCIES
  sqlite3
)

set(SOURCES
  ui.c
)

add_executable(ui
  ${SOURCES}
)

add_dependencies(ui
  ${DEPENDENCIES}
)

target_include_directories(ui PUBLIC
  ${CMAKE_SOURCE_DIR}
  ${GTK2_INCLUDE_DIRS}
)

target_link_libraries(ui
  ${DEPENDENCIES}
  ${GTK2_LIBRARIES}
)
