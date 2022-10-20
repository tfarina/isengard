#ifndef _OS_PATH_H_INCLUDED_
#define _OS_PATH_H_INCLUDED_ 1

/**
 * Returns the last element of path.
 *
 * @param path The path to get the last element of
 * @return The last element of the path
 */
char const *os_path_basename(char const *path);

#endif  /* !defined(_OS_PATH_H_INCLUDED_) */
