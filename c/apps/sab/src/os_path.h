#ifndef _OS_PATH_H_INCLUDED_
#define _OS_PATH_H_INCLUDED_ 1

/**
 * Returns a pointer to the last element of the path.
 *
 * @param[in] path A pointer to a string that contains the path to search.
 * @return Returns a pointer to the last element of the path if successful.
 */
char const *os_path_basename(char const *path);

/**
 * Joins directory with file name into a single path, separating them with an
 * Linux specific separator.
 *
 * @param[in] dir A pointer to a string that contains the directory path.
 * @param[in] file A pointer to a string that contains the file name.
 */
char *os_path_join(char const *dir, char const *file);

/**
 * Determines whether the given path (such as a file or directory) exists.
 *
 * @param[in] path A pointer to a string that contains the path to verify.
 * @return Returns true if the path exists; otherwise, false.
 */
int os_path_exists(char const *path);

/**
 * Verifies that the given path is a directory.
 *
 * @param[in] path A pointer to a string that contains the path to verify.
 * @return Returns true if path is a directory; otherwise, false.
 */

int os_path_isdir(char const *path);

#endif  /* !defined(_OS_PATH_H_INCLUDED_) */
