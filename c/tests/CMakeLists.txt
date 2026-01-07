# Add third_party/utest to the include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../third_party)

set(SRCS
  os_path_test.c
  run_all_unittests.c
)

add_executable(common_unittests ${SRCS})

# Link with common code
target_link_libraries(common_unittests common)
