set(TARGETNAME "${PROJECT_NAME}cli")

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

add_executable(${TARGETNAME}
  ${SOURCES}
)

add_dependencies(${TARGETNAME}
  ${DEPENDENCIES}
)

target_include_directories(${TARGETNAME} PUBLIC
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${TARGETNAME}
  ${DEPENDENCIES}
)

set(TARGETNAME "${PROJECT_NAME}add")

set(SOURCES
  add.c
)

add_executable(${TARGETNAME}
  ${SOURCES}
)

target_link_libraries(${TARGETNAME}
  ${DEPENDENCIES}
)

set(TARGETNAME "${PROJECT_NAME}mod")

set(SOURCES
  mod.c
)

add_executable(${TARGETNAME}
  ${SOURCES}
)

target_link_libraries(${TARGETNAME}
  ${DEPENDENCIES}
)

set(TARGETNAME "${PROJECT_NAME}del")

set(SOURCES
  del.c
)

add_executable(${TARGETNAME}
  ${SOURCES}
)

target_link_libraries(${TARGETNAME}
  ${DEPENDENCIES}
)

set(TARGETNAME "${PROJECT_NAME}ls")

set(SOURCES
  fort.c
  common.c
  ls.c
)

add_executable(${TARGETNAME}
  ${SOURCES})

target_link_libraries(${TARGETNAME}
  ${DEPENDENCIES}
)
