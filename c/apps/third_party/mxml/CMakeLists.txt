cmake_minimum_required(VERSION 3.5)
project(mxml C)

include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFiles)

set(MXML_VERSION "Mini-XML v3.0")

check_type_size("long long int" HAVE_LONG_LONG)

check_function_exists(snprintf HAVE_SNPRINTF)
check_function_exists(vsnprintf HAVE_VSNPRINTF)
check_function_exists(strdup HAVE_STRDUP)
check_function_exists(strlcat HAVE_STRLCAT)
check_function_exists(strlcpy HAVE_STRLCPY)

if (NOT WIN32)
  check_include_files(pthread.h HAVE_PTHREAD_H)
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

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
