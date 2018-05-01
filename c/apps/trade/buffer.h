#ifndef BUFFER_H_
#define BUFFER_H_

#include <stddef.h>

typedef struct {
  char *data;
  size_t length;
} buffer_t;

void buffer_init(buffer_t *buf);

void buffer_add(buffer_t *buf, char *data, size_t size);

#endif /* BUFFER_H_ */
