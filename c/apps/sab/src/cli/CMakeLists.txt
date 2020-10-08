set(EXE_NAME "sab")

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

add_executable(${EXE_NAME}
  ${SOURCES}
)

add_dependencies(${EXE_NAME}
  ${DEPENDENCIES}
)

target_include_directories(${EXE_NAME} PUBLIC
  ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${EXE_NAME}
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
  sab-showrec.c
)

add_executable(sab-showrec
  ${SOURCES})

target_link_libraries(sab-showrec
  ${DEPENDENCIES}
)
