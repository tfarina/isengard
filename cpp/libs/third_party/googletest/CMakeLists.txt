add_library(googletest src/gtest-all.cc)
target_include_directories(googletest PUBLIC
  include
)
target_link_libraries(googletest
  pthread
)

add_library(googletest-main src/gtest_main.cc)
target_include_directories(googletest-main PUBLIC
  include
)
