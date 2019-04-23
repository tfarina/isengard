cmake_minimum_required(VERSION 2.8)
project(moneybin C)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c99 -pedantic")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug")
endif()
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  set(CMAKE_C_FLAGS "-g -O2")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(CMAKE_C_FLAGS "-g -O0")
endif()

add_definitions(-D_GNU_SOURCE)

add_subdirectory(third_party/iniparser)
add_subdirectory(third_party/libcsv)
add_subdirectory(third_party/sqlite)

option(WITH_MYSQL "Use MySQL library" ON)

include(FindMySQL)
find_package(MySQL REQUIRED)

set(SRCS
  backend_mysql.c
  buffer.c
  csv_helper.c
  curl_write_callbacks.c
  config.c
  dba.c
  ffileutils.c
  fstrutils.c
  futils.c
  stats.c
  vector.c
)

add_library(moneybin ${SRCS})
target_link_libraries(moneybin iniparser libcsv ${MYSQL_LIBRARIES})

add_executable(csvcat csvcat.c)
target_link_libraries(csvcat libcsv moneybin ${MYSQL_LIBRARIES})

add_executable(csvstats csvstats.c)
target_link_libraries(csvstats libcsv moneybin -lm ${MYSQL_LIBRARIES})

find_package(CURL REQUIRED)

if (CURL_FOUND)
  include_directories(${CURL_INCLUDE_DIRS})

  add_executable(csvget csvget.c)
  target_link_libraries(csvget moneybin ${CURL_LIBRARIES} ${MYSQL_LIBRARIES})
endif()

add_executable(csvimport csvimport.c)
target_link_libraries(csvimport iniparser libcsv moneybin ${MYSQL_LIBRARIES})

add_executable(lastdate lastdate.c)
target_link_libraries(lastdate iniparser moneybin ${MYSQL_LIBRARIES})
