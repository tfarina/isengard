cmake_minimum_required(VERSION 2.6)
project(apps C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -std=c99 -pedantic")

add_definitions(-D_GNU_SOURCE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(MySQL)

add_subdirectory(third_party/iniparser)

set(LIBCSV_SRCS
  libcsv.c
)

add_library(libcsv ${LIBCSV_SRCS})

add_executable(tst tst.c)
target_link_libraries(tst libcsv)

add_executable(readfile readfile.c)
add_executable(writefile writefile.c)
add_executable(buildtime buildtime.c)
add_executable(linebyline linebyline.c)
add_executable(stream_test stream.c stream_test.c)
add_executable(math_util_test math_util.c math_util_test.c)
target_link_libraries(math_util_test -lm)
add_executable(differentiation differentiation.c)
target_link_libraries(differentiation -lm)
add_executable(power2 power2.c)
add_executable(cansse2 cansse2.c)
add_executable(lexer lexer.c)
add_executable(logging lookup.c logging.c)
add_executable(lookup_test lookup.c lookup_test.c)
add_executable(execprog execprog.c)
add_executable(getcwd getcwd.c)
add_executable(exampled exampled.c)
add_executable(strdup strutils.c strdup.c)
add_executable(ascii_table ascii_table.c)
add_executable(bit bit.c)
add_executable(homedir futils.c homedir.c)
add_executable(username futils.c username.c)
add_executable(cpucount cpucount.c)
add_executable(ascii ascii.c)
add_executable(cjson_test cJSON.c cJSONTest.c)
target_link_libraries(cjson_test -lm)
add_executable(pointers pointers.c)
add_executable(pointers2 pointers2.c)
add_executable(arrays_n_pointers arrays_n_pointers.c)
add_executable(postfixoperator postfixoperator.c)
add_executable(inheritance inheritance.c)
add_executable(arraysize arraysize.c)
add_executable(readjson cJSON.c readjson.c)
target_link_libraries(readjson -lm)
add_executable(readmenujson cJSON.c readmenujson.c)
target_link_libraries(readmenujson -lm)
add_executable(select select.c)
add_executable(percentage percentage.c)
add_executable(growth_rate growth_rate.c)
target_link_libraries(growth_rate -lm)
add_executable(future_value future_value.c)
target_link_libraries(future_value -lm)
add_executable(present_value present_value.c)
target_link_libraries(present_value -lm)
add_executable(pivot pivot.c)
add_executable(querydate querydate.c)
add_executable(sizedt sizedt.c)
add_executable(cprimitivedatatypes cprimitivedatatypes.c)
add_executable(ccustomdatatypes ccustomdatatypes.c)
add_executable(ctypequalifiers ctypequalifiers.c)
add_executable(cstorageclassspecifiers cstorageclassspecifiers.c)
add_executable(ckeywords ckeywords.c)
add_executable(stats_test stats_test.c stats.c)
target_link_libraries(stats_test -lm)
add_executable(mysql_version mysql_version.c)
target_link_libraries(mysql_version ${MYSQL_LIBRARY})



add_executable(ticks ticks.c)
target_link_libraries(ticks libcsv)

add_executable(csvinfo csvinfo.c)
target_link_libraries(csvinfo libcsv)

set(LIBCSV_PARSER_SRCS
  csv_parser.c
)

add_library(libcsv_parser ${LIBCSV_PARSER_SRCS})
add_executable(csvimport csvimport.c)
target_link_libraries(csvimport libcsv_parser)
