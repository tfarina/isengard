#ifndef F_FILE_UTILS_H_
#define F_FILE_UTILS_H_

#include <stddef.h>

char *f_build_filename(const char *directory, const char *name);

int f_read_file(const char *filename, char **contents, size_t *out_file_size);

int f_write_file(const char *filename, const char *data, size_t size);

#endif  /* F_FILE_UTILS_H_ */
