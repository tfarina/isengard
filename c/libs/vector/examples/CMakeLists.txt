set(SRCS
  vector_example.c
)

add_executable(vector_example ${SRCS})
target_include_directories(vector_example PUBLIC
  ${PROJECT_SOURCE_DIR}
)
target_link_libraries(vector_example vector)
