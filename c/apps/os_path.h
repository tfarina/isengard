#ifndef _OS_PATH_H_INCLUDED_
#define _OS_PATH_H_INCLUDED_ 1

/**
 * Returns the last element of path.
 */
char const *os_path_basename(char const *path);

char *os_path_join(char *dir, char *file);

#endif  /* !defined(_OS_PATH_H_INCLUDED_) */
