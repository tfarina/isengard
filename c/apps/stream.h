#ifndef STREAM_H_
#define STREAM_H_

#include <stdio.h>
#include <stddef.h>

int stream_read_line(FILE *file, char *data, size_t size, size_t *outlen);

#endif /* STREAM_H_ */
