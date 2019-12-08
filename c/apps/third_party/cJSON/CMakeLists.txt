cmake_minimum_required(VERSION 2.6)
project(cJSON C)

set(SRCS
    cJSON.c)

add_library(cJSON ${SRCS})

option(ENABLE_CJSON_TEST "Enable building cJSON test" ON)
if (ENABLE_CJSON_TEST)
  set(TEST_CJSON cJSON_test)
  add_executable(cJSON_test cJSONTest.c)
  target_link_libraries(cJSON_test cJSON -lm)
endif()
