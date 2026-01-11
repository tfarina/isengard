#ifndef FFILEUTILS_H_
#define FFILEUTILS_H_

#include <stddef.h>

int f_file_size(char const *path);

char *f_read_file(const char *filename, size_t *out_file_size);

#endif  /* FFILEUTILS_H_ */
