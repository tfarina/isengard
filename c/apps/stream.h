#ifndef STREAM_H_
#define STREAM_H_

#include <stdio.h>
#include <stddef.h>

#define STREAM_EOK 0
#define STREAM_EIO 1

int stream_read_line(FILE *file, char *data, size_t size, size_t *outlen);

#endif /* STREAM_H_ */
