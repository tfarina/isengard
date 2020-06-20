#ifndef FILEUTILS_H_
#define FILEUTILS_H_

#include <stddef.h>

char *read_file(const char *filename, size_t *rlen);

int write_file(const char *filename, const char *data, size_t size);

#endif /* FILEUTILS_H_ */
