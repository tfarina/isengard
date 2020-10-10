set(EXENAME "${PROJECT_NAME}")

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
  os_path.c
)

add_executable(${EXENAME}
  ${SOURCES}
)

add_dependencies(${EXENAME}
  ${DEPENDENCIES}
)

target_include_directories(${EXENAME} PUBLIC
  ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${EXENAME}
  ${DEPENDENCIES}
)

set(SOURCES
  sab-add.c
)

add_executable(sab-add
  ${SOURCES}
)

target_link_libraries(sab-add
  ${DEPENDENCIES}
)

set(SOURCES
  sab-change.c
)

add_executable(sab-change
  ${SOURCES}
)

target_link_libraries(sab-change
  ${DEPENDENCIES}
)

set(SOURCES
  sab-delete.c
)

add_executable(sab-delete
  ${SOURCES}
)

target_link_libraries(sab-delete
  ${DEPENDENCIES}
)

set(SOURCES
  fort.c
  common.c
  sab-ls.c
)

add_executable(sab-ls
  ${SOURCES})

target_link_libraries(sab-ls
  ${DEPENDENCIES}
)
