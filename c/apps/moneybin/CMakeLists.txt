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
add_subdirectory(third_party/mxml)
add_subdirectory(third_party/zlib)

option(WITH_MYSQL "Use MySQL library" ON)

if (WITH_MYSQL)
  include(FindMySQL)
  find_package(MySQL REQUIRED)
  if (MYSQL_FOUND)
    add_definitions(-DHAVE_MYSQL)
    set(LIBS ${LIBS} ${MYSQL_LIBRARIES})
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
target_link_libraries(moneybin iniparser libconfigini ini libcsv ${LIBS})

add_executable(csvcat sfldate.c timestamp.c tacsv.c fstrutils.c csvcat.c)
target_link_libraries(csvcat libcsv)

add_executable(csvstats sfldate.c timestamp.c fstrutils.c stats.c tacsv.c csvstats.c)
target_link_libraries(csvstats libcsv -lm)

add_executable(sma_test sma.c sma_test.c)
add_executable(gensignals sfldate.c timestamp.c crossover.c sma.c tacsv.c gensignals.c)
target_link_libraries(gensignals libcsv)

add_executable(genmovavgs sfldate.c timestamp.c crossover.c sma.c tacsv.c genmovavgs.c)
target_link_libraries(genmovavgs libcsv)

add_executable(genexpma sfldate.c timestamp.c ema.c tacsv.c genexpma.c)
target_link_libraries(genexpma libcsv)

add_executable(calcpivot sfldate.c timestamp.c tacsv.c pivot.c calcpivot.c)
target_link_libraries(calcpivot libcsv)

add_executable(ema_test ema.c ema_test.c)

add_executable(calcrsi sfldate.c timestamp.c tacsv.c calcrsi.c)
target_link_libraries(calcrsi libcsv)

find_package(CURL REQUIRED)

add_executable(csvget iso8601.c csvget.c)
target_link_libraries(csvget moneybin ${CURL_LIBRARIES})
target_include_directories(csvget PRIVATE ${CURL_INCLUDE_DIRS})

add_executable(csvimport csvimport.c)
target_link_libraries(csvimport iniparser libcsv moneybin ${LIBS})

add_executable(lastdate lastdate.c)
target_link_libraries(lastdate iniparser moneybin ${LIBS})

add_executable(startdate startdate.c)
target_link_libraries(startdate iniparser moneybin ${LIBS})

add_executable(enddate enddate.c)

add_executable(strtotime iso8601.c strtotime.c)

add_executable(dadosempresa dadosempresa.c)
target_link_libraries(dadosempresa mxml)

add_executable(fetchdfp download.c dounzip.c fetchdfp.c)
target_link_libraries(fetchdfp zlib moneybin ${CURL_LIBRARIES})
target_include_directories(fetchdfp PRIVATE ${CURL_INCLUDE_DIRS})

add_executable(infofina infofina.c)
target_link_libraries(infofina mxml)

add_executable(dbinit dbinit.c)
target_include_directories(dbinit PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(dbinit sqlite3 moneybin)
