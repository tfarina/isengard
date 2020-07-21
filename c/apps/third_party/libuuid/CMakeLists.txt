cmake_minimum_required(VERSION 3.5)
project(libuuid C)

add_definitions(-DHAVE_CONFIG_H)

set(SRCS
  clear.c
  compare.c
  copy.c
  gen_uuid.c
  isnull.c
  pack.c
  parse.c
  randutils.c
  unpack.c
  unparse.c
  uuid_time.c
)

add_library(uuid STATIC ${SRCS})

add_executable(test_uuid test_uuid.c)
target_link_libraries(test_uuid uuid)
