add_library(common STATIC os_path.c)

# This exposes os_path.h to anything that links against common.
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
