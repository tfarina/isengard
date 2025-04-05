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

install(TARGETS ${TARGETNAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
