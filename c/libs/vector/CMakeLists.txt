# Description
# ===========
#
#   Use:
#       Move to a custom directory, ideally out of source.
#
#   Example:
#       For example, in the project directory, to build the library
#       with debug configuration, type the following:
#           mkdir -p out/debug && cd out/debug
#           cmake -GNinja -DCMAKE_BUILD_TYPE=Debug ../..
#
cmake_minimum_required(VERSION 2.8)
project(vector C)

set(VECTORLIB_MAJOR 0)
set(VECTORLIB_MINOR 1)
set(VECTORLIB_PATCH 0)
set(VECTORLIB_VERSION ${VECTORLIB_MAJOR}.${VECTORLIB_MINOR}.${VECTORLIB_PATCH})

include(GNUInstallDirs)

set(SRCS
  vector.c
)

set(HDRS
  vector.h
)

add_library(vector ${HDRS} ${SRCS})

add_subdirectory(examples)

enable_testing()
add_subdirectory(tests)

find_package(Doxygen)

if (DOXYGEN_FOUND)
  configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

  add_custom_target(doc-doxygen ALL
                    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generation API documentation with Doxygen"
                    VERBATIM)
endif()

# libvector.pc (pkg-config file).
set(libdir     "${CMAKE_INSTALL_FULL_LIBDIR}")
set(includedir "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
configure_file(libvector.pc.in libvector.pc @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libvector.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

install(FILES ${HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS vector
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

#
# Summary
#
message(STATUS "=============================================================")
message(STATUS "")
message(STATUS "Configuration")
message(STATUS "  Build type ............. ${CMAKE_BUILD_TYPE}")
message(STATUS "  Shared libraries ....... ${BUILD_SHARED_LIBS}")
message(STATUS "  C Compiler ............. ${CMAKE_C_COMPILER}")
message(STATUS "  Compiler flags")
message(STATUS "    C .................... ${CMAKE_C_FLAGS}")
message(STATUS "  Linker flags")
message(STATUS "    Executable ........... ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "    Module ............... ${CMAKE_MODULE_LINKER_FLAGS}")
message(STATUS "    Shared ............... ${CMAKE_SHARED_LINKER_FLAGS}")
message(STATUS "  Paths")
message(STATUS "    Install prefix ....... ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "=============================================================")
