#ifndef _OS_PATH_H_INCLUDED_
#define _OS_PATH_H_INCLUDED_ 1

/**
 * Returns a pointer to the last element of the path.
 *
 * @param path The path to get the last element of
 * @return The last element of the path
 */
char const *os_path_basename(char const *path);

/**
 * Joins directory with file name into a single path, separating them with an
 * Linux specific separator.
 *
 * @param[in] dir A directory path.
 * @param[in] file A file name.
 */
char *os_path_join(char *dir, char *file);

#endif  /* !defined(_OS_PATH_H_INCLUDED_) */
