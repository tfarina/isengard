# DistTarget.cmake
#
# Defines custom targets related to distributing source code.
# It requires to have populated 'PACKAGE' and 'VERSION'
# variables. It also uses 'PROJECT_DISTCONFIGURE_PARAMS' variable when
# configuring the unpacked distribution.
#
# Added targets:
# dist - only creates a tarball
# disttest - creates a tarball and 'make && make install' it to a temporary prefix
#    to verify that the code can be built and installed; it also verifies
#    that the first line of the NEWS file contains the same version as
#    the tarball and that it claims today's date.
# distcheck - similar to 'disttest', only runs also 'make check' before installing
# NOTE: disttest, and distcheck are not implemented/imported yet from evolution

# Filenames for tarball
set(ARCHIVE_NAME ${PACKAGE}-${VERSION})

add_custom_target(dist
                  COMMAND ${CMAKE_COMMAND} -E echo "Creating '${ARCHIVE_NAME}.tar.gz'..."
                  COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD | gzip > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.gz
                  COMMAND ${CMAKE_COMMAND} -E echo "Distribution tarball '${ARCHIVE_NAME}.tar.gz' created at ${CMAKE_BINARY_DIR}"
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
