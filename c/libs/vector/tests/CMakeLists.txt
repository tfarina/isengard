add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

set(EXENAME vector_unittests)

set(HDRS
  greatest.h
)

set(SRCS
  unittests.c
  vector_test.c
)

add_executable(${EXENAME} ${HDRS} ${SRCS})

target_include_directories(${EXENAME} PUBLIC
  ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${EXENAME} vector)

add_test(
    NAME ${EXENAME}
    COMMAND $<TARGET_FILE:${EXENAME}>
)

add_dependencies(check ${EXENAME})
