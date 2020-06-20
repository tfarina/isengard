#ifndef FFILEUTILS_H_
#define FFILEUTILS_H_

#include <stddef.h>

char *build_filename(char *dirname, char *filename);

char *f_read_file(const char *filename, size_t *rlen);

int f_write_file(const char *filename, const char *data, size_t size);

#endif /* FFILEUTILS_H_ */
