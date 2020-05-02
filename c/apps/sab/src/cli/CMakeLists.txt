set(DEPENDENCIES
  ab
)

set(SOURCES
  cmd_add.c
  cmd_change.c
  cmd_delete.c
  cmd_list.c
  commands.h
  common.c
  fort.c
  main.c
)

add_executable(sab
  ${SOURCES}
)

add_dependencies(sab
  ${DEPENDENCIES}
)

target_include_directories(sab PUBLIC
  ${CMAKE_SOURCE_DIR}
)

target_link_libraries(sab
  ${DEPENDENCIES}
)
