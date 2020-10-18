#ifndef BUFFER_H_
#define BUFFER_H_

#include <stddef.h>

typedef struct {
  char *data;
  size_t size;
} buffer_t;

void buffer_init(buffer_t *buf);

void buffer_append(buffer_t *buf, char *data, size_t size);

#endif /* BUFFER_H_ */
