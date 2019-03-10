#ifndef FILE_H_
#define FILE_H_

#include <stddef.h>

int read_file(const char *filename, char **contents, size_t *len);

int write_file(const char *filename, const char *data, size_t size);

#endif /* FILE_H_ */
