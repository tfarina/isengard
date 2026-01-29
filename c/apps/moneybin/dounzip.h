#ifndef DOUNZIP_H
#define DOUNZIP_H

/**
 * Extract files from a zip archive.
 *
 * @param zipfile   The pathname of the zip file.
 * @param exdir     The directory to extract files to.
 */
int unzip(char const *zipfile, char const *exdir);

#endif /* DOUNZIP_H */
