cmake_minimum_required(VERSION 3.5)
project(mxml C)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(SRCS
  mxml-attr.c
  mxml-entity.c
  mxml-file.c
  mxml-get.c
  mxml-index.c
  mxml-node.c
  mxml-private.c
  mxml-search.c
  mxml-set.c
  mxml-string.c
)

add_library(mxml STATIC ${SRCS})

add_executable(testmxml testmxml.c)
target_link_libraries(testmxml mxml -pthread)
