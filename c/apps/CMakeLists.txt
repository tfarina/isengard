cmake_minimum_required(VERSION 2.6)
project(apps C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -std=c99 -pedantic")

add_definitions(-D_GNU_SOURCE)

# Add local cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

set(M_LIBRARY "-lm")

add_subdirectory(third_party/iniparser)
add_subdirectory(third_party/libcsv)
add_subdirectory(third_party/cJSON)
add_subdirectory(third_party/mxml)
add_subdirectory(third_party/libuuid)
add_subdirectory(third_party/clark)
add_subdirectory(third_party/libxlsxwriter)

add_executable(tst tst.c)
target_link_libraries(tst libcsv)

add_executable(ticks ticks.c)
target_link_libraries(ticks libcsv)

add_executable(readfile ffileutils.c readfile.c)
add_executable(writefile ffileutils.c writefile.c)
add_executable(buildtime buildtime.c)
add_executable(stream_test stream.c stream_test.c)
add_executable(math_util_test math_util.c math_util_test.c)
target_link_libraries(math_util_test ${M_LIBRARY})
add_executable(differentiation differentiation.c)
target_link_libraries(differentiation ${M_LIBRARY})
add_executable(square square.c)
add_executable(cansse2 cpu.c cansse2.c)
add_executable(logging lookup.c logging.c)
add_executable(lookup_test lookup.c lookup_test.c)
add_executable(execprog execprog.c)
add_executable(getcwd getcwd.c)
add_executable(exampled exampled.c)
add_executable(strdup xstring.c strdup.c)
add_executable(ascii_table ascii_table.c)
add_executable(bit bit.c)
add_executable(bitmask bitmask.c)
add_executable(homedir os_path.c user.c homedir.c)
add_executable(username id.c username.c)
add_executable(myhostname third_party/xgethostname/xgethostname.c myhostname.c)
add_executable(configdir os_path.c user.c configdir.c)
add_executable(datadir os_path.c user.c datadir.c)
add_executable(cpucount cpu.c cpucount.c)
add_executable(ascii ascii.c)
add_executable(inheritance inheritance.c)
add_executable(arraysize_test arraysize_test.c)
add_executable(readjson ffileutils.c readjson.c)
target_link_libraries(readjson cjson)
add_executable(readmenujson ffileutils.c readmenujson.c)
target_link_libraries(readmenujson cjson)
add_executable(select select.c)
add_executable(percentage_example percentage.c percentage_example.c)
target_link_libraries(percentage_example ${M_LIBRARY})
add_executable(growth_rate financial.c growth_rate.c)
target_link_libraries(growth_rate ${M_LIBRARY})
add_executable(future_value financial.c future_value.c)
target_link_libraries(future_value ${M_LIBRARY})
add_executable(present_value financial.c present_value.c)
target_link_libraries(present_value ${M_LIBRARY})
add_executable(pivot_test pivot.c pivot_test.c)
add_executable(querydate querydate.c)
add_executable(sizeof sizeof.c)
add_executable(sizemax sizemax.c)
add_executable(cprimitivedatatypes cprimitivedatatypes.c)
add_executable(ccustomdatatypes ccustomdatatypes.c)
add_executable(ctypequalifiers ctypequalifiers.c)
add_executable(cstorageclassspecifiers cstorageclassspecifiers.c)
add_executable(ckeywords ckeywords.c)
add_executable(stats_test stats_test.c stats.c)
target_link_libraries(stats_test ${M_LIBRARY})
add_executable(ansi_colors_test ansi_colors_test.c)
add_executable(pidfile pidfile.c)
add_executable(buildpath os_path.c buildpath.c)
add_executable(mkdirp_test xstring.c os_path.c mkdirp.c mkdirp_test.c)
add_executable(os_path_test os_path.c os_path_test.c)
add_executable(wiyname wiyname.c)
add_executable(yesno yesno.c)

set(LIBCSV_PARSER_SRCS
  csv_parser.c
)

add_library(libcsv_parser ${LIBCSV_PARSER_SRCS})
add_executable(csvimport csvimport.c)
target_link_libraries(csvimport libcsv_parser)

find_package(GTK2 2.6)

if (GTK2_FOUND)
  add_executable(graph graph.c)

  target_include_directories(graph PUBLIC ${GTK2_INCLUDE_DIRS})

  target_link_libraries(graph ${M_LIBRARY} ${GTK2_LIBRARIES})
endif()

# These options are settable from the CMake command line. For example, to disable
# SQL, put -DWITH_MYSQL=OFF on the command line.

option(WITH_MYSQL "Include MySQL support" OFF)

if (WITH_MYSQL)
  find_package(MySQL)
  add_executable(mysql_version mysql_version.c)
  target_link_libraries(mysql_version ${MYSQL_LIBRARY})
endif()

add_executable(writexml writexml.c)
target_link_libraries(writexml mxml -pthread)

add_executable(readxml readxml.c)
target_link_libraries(readxml mxml -pthread)

add_executable(readxml4 readxml4.c)
target_link_libraries(readxml4 mxml -pthread)

add_executable(writexml2 writexml2.c)
target_link_libraries(writexml2 mxml -pthread)

add_executable(genuuid genuuid.c)
target_link_libraries(genuuid uuid)

add_executable(writexml3 writexml3.c)
target_link_libraries(writexml3 mxml uuid -pthread)

add_executable(writexml4 xstring.c alpm_list.c writexml4.c)
target_link_libraries(writexml4 mxml uuid -pthread)

add_executable(hashtable_test hashtable_test.c)
target_link_libraries(hashtable_test chashtable uuid ${M_LIBRARY})

add_executable(encode base64.c encode.c)

add_executable(tempdir_test xstring.c os_path.c tempdir.c tempdir_test.c)
add_executable(getshell getshell.c)

add_executable(example1 third_party/pbPlots/pbPlots.c third_party/pbPlots/supportLib.c third_party/pbPlots/example1.c)
target_link_libraries(example1 ${M_LIBRARY})
add_executable(example2 third_party/pbPlots/pbPlots.c third_party/pbPlots/supportLib.c third_party/pbPlots/example2.c)
target_link_libraries(example2 ${M_LIBRARY})
add_executable(example3 third_party/pbPlots/pbPlots.c third_party/pbPlots/supportLib.c third_party/pbPlots/example3.c)
target_link_libraries(example3 ${M_LIBRARY})
add_executable(example4 third_party/pbPlots/pbPlots.c third_party/pbPlots/supportLib.c third_party/pbPlots/example4.c)
target_link_libraries(example4 ${M_LIBRARY})

add_executable(structsize structsize.c)

add_executable(hello_excel hello_excel.c)
target_link_libraries(hello_excel libxlsxwriter)

add_executable(celtofah temperature.c celtofah.c)
add_executable(fahtocel temperature.c fahtocel.c)
add_executable(bhaskara bhaskara.c)
target_link_libraries(bhaskara ${M_LIBRARY})

add_executable(year year.c)
add_executable(mday mday.c)
add_executable(mon mon.c)
add_executable(weekday weekday.c)
add_executable(longdate longdate.c)
add_executable(time time.c)

add_executable(countlines xstring.c countlines.c)

add_executable(numbers os_path.c xstring.c numbers.c)
target_link_libraries(numbers)
add_executable(numbersv2 os_path.c xstring.c numbersv2.c)
target_link_libraries(numbersv2)

add_executable(degrees angle.c degrees.c)
add_executable(radians angle.c radians.c)

add_executable(acceptanddisplay acceptanddisplay.c)
add_executable(promptuser promptuser.c)
add_executable(promptpassword promptpassword.c)

add_executable(freenull freenull.c)

add_executable(atm atm.c)

find_package(PkgConfig REQUIRED)

set(ID3TAG_MINIMUM_VERSION 0.15.0)
set(TAGLIB_MINIMUM_VERSION 1.9.1)

# Check for required packages
pkg_check_modules(ID3TAG REQUIRED
  id3tag>=${ID3TAG_MINIMUM_VERSION}
)
pkg_check_modules(TAGLIB REQUIRED
  taglib_c>=${TAGLIB_MINIMUM_VERSION}
)

add_executable(songinfo songinfo.c)
target_compile_options(songinfo PUBLIC
  ${ID3TAG_CFLAGS}
)
target_include_directories(songinfo PUBLIC
  ${ID3TAG_INCLUDE_DIRS}
)
target_link_libraries(songinfo
  ${ID3TAG_LDFLAGS}
)

add_executable(songinfov2 songinfov2.c)
target_compile_options(songinfov2 PUBLIC
  ${TAGLIB_CFLAGS}
)
target_include_directories(songinfov2 PUBLIC
  ${TAGLIB_INCLUDE_DIRS}
)
target_link_libraries(songinfov2
  ${TAGLIB_LDFLAGS}
)

add_executable(songplay songplay.c)
target_link_libraries(songplay
  -pthread
  ${CMAKE_DL_LIBS}
  ${M_LIBRARY}
)

set(PNG_MINIMUM_VERSION 1.2.54)

pkg_check_modules(PNG REQUIRED
  libpng>=${PNG_MINIMUM_VERSION}
)
add_executable(scanimg scanimg.c)
target_link_libraries(scanimg
  png
)
