cmake_minimum_required(VERSION 2.6)
project(cJSON C)

#cJSON
set(CJSON_LIB cjson)

file(GLOB HEADERS cJSON.h)
set(SOURCES cJSON.c)

add_library("${CJSON_LIB}" "${CJSON_LIBRARY_TYPE}" "${HEADERS}" "${SOURCES}")
if (NOT WIN32)
  target_link_libraries("${CJSON_LIB}" m)
endif()

option(ENABLE_CJSON_TEST "Enable building cJSON test" ON)
if (ENABLE_CJSON_TEST)
  set(TEST_CJSON cJSON_test)
  add_executable("${TEST_CJSON}" cJSONTest.c)
  target_link_libraries("${TEST_CJSON}" "${CJSON_LIB}")
endif()
