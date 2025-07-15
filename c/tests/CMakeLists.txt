# Add third_party/utest to the include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../third_party)

# Define test executable
add_executable(os_path_test os_path_test.c)

# Link with common code
target_link_libraries(os_path_test common)
