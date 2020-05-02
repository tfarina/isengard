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

set(SOURCES
  add.c
)

add_executable(add
  ${SOURCES}
)

target_link_libraries(add
  ${DEPENDENCIES}
)

set(SOURCES
  change.c
)

add_executable(change
  ${SOURCES}
)

target_link_libraries(change
  ${DEPENDENCIES}
)

set(SOURCES
  delete.c
)

add_executable(delete
  ${SOURCES}
)

target_link_libraries(delete
  ${DEPENDENCIES}
)

set(SOURCES
  fort.c
  common.c
  showrec.c
)

add_executable(showrec
  ${SOURCES})

target_link_libraries(showrec
  ${DEPENDENCIES}
)
