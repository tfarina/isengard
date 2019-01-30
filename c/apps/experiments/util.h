#ifndef UTIL_H_
#define UTIL_H_

/**
 * Gets the current user's home directory.
 */
const char *f_get_home_dir(void);

char *f_build_filename(const char *directory, const char *name);

#endif  /* UTIL_H_ */
