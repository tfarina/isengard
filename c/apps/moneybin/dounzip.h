#ifndef _DOUNZIP_H_INCLUDED_
#define _DOUNZIP_H_INCLUDED_ 1

/**
 * Extract files from a zip archive.
 *
 * @param zipfile   The pathname of the zip file.
 * @param exdir     The directory to extract files to.
 */
int unzip(char const *zipfile, char const *exdir);

#endif  /* !defined(_DOUNZIP_H_INCLUDED_) */
