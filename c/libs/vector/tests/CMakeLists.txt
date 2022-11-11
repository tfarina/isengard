set(HDRS
  greatest.h
)

set(SRCS
  unittests.c
  vector_test.c
)

add_executable(unittests ${HDRS} ${SRCS})

target_include_directories(unittests PUBLIC
  ${PROJECT_SOURCE_DIR}
)

target_link_libraries(unittests vector)

add_test(
    NAME unittests
    COMMAND $<TARGET_FILE:unittests>
)
