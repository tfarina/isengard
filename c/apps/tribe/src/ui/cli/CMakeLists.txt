set(EXENAME "${PROJECT_NAME}cli")

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
  add.c
)

add_executable(${PROJECT_NAME}add
  ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}add
  ${DEPENDENCIES}
)

set(SOURCES
  mod.c
)

add_executable(${PROJECT_NAME}mod
  ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}mod
  ${DEPENDENCIES}
)

set(SOURCES
  del.c
)

add_executable(${PROJECT_NAME}del
  ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}del
  ${DEPENDENCIES}
)

set(SOURCES
  fort.c
  common.c
  ls.c
)

add_executable(${PROJECT_NAME}ls
  ${SOURCES})

target_link_libraries(${PROJECT_NAME}ls
  ${DEPENDENCIES}
)
