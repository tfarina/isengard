#ifndef FFILEUTILS_H_
#define FFILEUTILS_H_

#include <stddef.h>

int file_size(char const *path);

char *read_file(const char *filename, size_t *rlen);

#endif  /* FFILEUTILS_H_ */

