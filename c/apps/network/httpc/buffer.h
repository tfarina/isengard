#ifndef BUFFER_H_
#define BUFFER_H_

#include <stddef.h>

typedef struct {
	char *data;
	size_t len;
	size_t cap;
} buffer_t;

buffer_t *buffer_alloc(size_t capacity);
void buffer_free(buffer_t *b);
void buffer_append(buffer_t *b, const void *data, size_t len);

#endif  /* BUFFER_H_ */
