cmake_minimum_required(VERSION 2.6)
project(xlsxio)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# Add local cmake modules
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${PROJECT_SOURCE_DIR}/cmake/modules)

find_package(EXPAT REQUIRED)

find_package(Minizip REQUIRED)

set(ANYZIP_DEF USE_MINIZIP)

set(HDRS
    xlsxio_private.h
    xlsxio_read.h
    xlsxio_read_sharedstrings.h
    xlsxio_version.h
)

set(SRCS
    xlsxio_read.c
    xlsxio_read_sharedstrings.c
)

add_library(xlsxio STATIC ${SRCS} ${HDRS})

set_target_properties(xlsxio PROPERTIES COMPILE_DEFINITIONS "${ANYZIP_DEF}")

target_link_libraries(xlsxio
  ${EXPAT_LIBRARIES}
  ${MINIZIP_LIBRARIES}
)
