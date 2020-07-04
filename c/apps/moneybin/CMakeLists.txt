cmake_minimum_required(VERSION 2.8)
project(moneybin C)

# Add local cmake modules
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${PROJECT_SOURCE_DIR}/cmake/modules)

include(UninstallTarget)

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
add_subdirectory(third_party/libconfigini)
add_subdirectory(third_party/ini)
add_subdirectory(third_party/libcsv)

option(WITH_MYSQL "Use MySQL library" ON)

if (WITH_MYSQL)
  include(FindMySQL)
  find_package(MySQL REQUIRED)
  if (MYSQL_FOUND)
    add_definitions(-DHAVE_MYSQL)
  else ()
    message("MySQL not found")
  endif()
endif()

option(WITH_SQLITE "Use SQLite3 library" ON)

if (WITH_SQLITE)
  add_subdirectory(third_party/sqlite)
endif()

set(SRCS
  sfldate.c
  timestamp.c
  backends/mysql.c
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
target_link_libraries(moneybin iniparser libconfigini ini libcsv ${MYSQL_LIBRARIES})

add_executable(csvcat fstrutils.c tacsv.c csvcat.c)
target_link_libraries(csvcat libcsv)

add_executable(csvstats fstrutils.c stats.c tacsv.c csvstats.c)
target_link_libraries(csvstats libcsv -lm)

add_executable(sma_test sma.c sma_test.c)
add_executable(trade vector.c crossover.c tacsv.c trade.c)
target_link_libraries(trade libcsv)

find_package(CURL REQUIRED)

add_executable(csvget csvget.c)
target_link_libraries(csvget moneybin ${CURL_LIBRARIES})
target_include_directories(csvget PUBLIC ${CURL_INCLUDE_DIRS})

add_executable(csvimport csvimport.c)
target_link_libraries(csvimport iniparser libcsv moneybin ${MYSQL_LIBRARIES})

add_executable(lastdate lastdate.c)
target_link_libraries(lastdate iniparser moneybin ${MYSQL_LIBRARIES})

add_executable(startdate startdate.c)
target_link_libraries(startdate iniparser moneybin ${MYSQL_LIBRARIES})

add_executable(enddate enddate.c)
