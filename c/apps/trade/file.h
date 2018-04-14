#ifndef FILE_H_
#define FILE_H_

#include <stddef.h>

char *readfile(char *filename, size_t *len);

int write_file(const char *filename, const char *data, size_t size);

#endif /* FILE_H_ */
