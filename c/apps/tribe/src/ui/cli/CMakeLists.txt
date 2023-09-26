set(EXENAME "${PROJECT_NAME}cmd")

set(DEPENDENCIES
  ab
)

set(SOURCES
  cmd_add.c
  cmd_modify.c
  cmd_delete.c
  cmd_list.c
  commands.h
  common.c
  fort.c
  main.c
)

add_executable(${EXENAME}
  ${SOURCES}
)

add_dependencies(${EXENAME}
  ${DEPENDENCIES}
)

target_include_directories(${EXENAME} PUBLIC
  ${CMAKE_BINARY_DIR}/src
  ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${EXENAME}
  ${DEPENDENCIES}
)

set(SOURCES
  tribecmd-add.c
)

add_executable(${EXENAME}-add
  ${SOURCES}
)

target_link_libraries(${EXENAME}-add
  ${DEPENDENCIES}
)

set(SOURCES
  tribecmd-modify.c
)

add_executable(${EXENAME}-modify
  ${SOURCES}
)

target_link_libraries(${EXENAME}-modify
  ${DEPENDENCIES}
)

set(SOURCES
  tribecmd-delete.c
)

add_executable(${EXENAME}-delete
  ${SOURCES}
)

target_link_libraries(${EXENAME}-delete
  ${DEPENDENCIES}
)

set(SOURCES
  fort.c
  common.c
  tribecmd-ls.c
)

add_executable(${EXENAME}-ls
  ${SOURCES})

target_link_libraries(${EXENAME}-ls
  ${DEPENDENCIES}
)
