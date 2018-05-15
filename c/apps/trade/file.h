#ifndef FILE_H_
#define FILE_H_

#include <stddef.h>

char *read_file(const char *filename, size_t *len);

int write_file(const char *filename, const char *data, size_t size);

char *make_file_path(const char *directory, const char *name);

#endif /* FILE_H_ */
