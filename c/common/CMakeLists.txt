add_library(common STATIC os_path.c)

# This exposes os_path.h to anything that links against common.
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add third_party to the include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../third_party)

set(TEST_SRCS
  os_path_test.c
  run_all_unittests.c
)

add_executable(common_unittests ${TEST_SRCS})

# Link with common code
target_link_libraries(common_unittests common)
