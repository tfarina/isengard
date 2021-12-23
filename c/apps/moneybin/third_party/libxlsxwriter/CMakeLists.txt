cmake_minimum_required(VERSION 2.6)
project(libxlsxwriter C)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

find_package(ZLIB REQUIRED "1.0")

set(HDRS

)

set(SRCS
    deps/minizip/ioapi.c
    deps/minizip/zip.c
    deps/tmpfileplus/tmpfileplus.c
    deps/md5/md5.c
    deps/dtoa/emyg_dtoa.c
    src/app.c
    src/chart.c
    src/chartsheet.c
    src/comment.c
    src/content_types.c
    src/core.c
    src/custom.c
    src/drawing.c
    src/format.c
    src/hash_table.c
    src/metadata.c
    src/packager.c
    src/relationships.c
    src/shared_strings.c
    src/styles.c
    src/table.c
    src/theme.c
    src/utility.c
    src/vml.c
    src/workbook.c
    src/worksheet.c
    src/xmlwriter.c
)

add_library(libxlsxwriter STATIC ${SRCS} ${HDRS})

target_include_directories(libxlsxwriter PUBLIC
  include
  include/xlsxwriter
  ${ZLIB_INCLUDE_DIRS}
)

target_link_libraries(libxlsxwriter LINK_PUBLIC
  ${ZLIB_LIBRARIES}
)
