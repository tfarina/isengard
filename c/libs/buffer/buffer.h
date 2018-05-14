#ifndef BUFFER_H_
#define BUFFER_H_

#include <stddef.h>

/*
 * A buffer_t is a generic memory buffer.
 */
typedef struct {
        /* The data contained in the buffer. */
	char *data;

        /* The current length of the buffer, the position for reading/writing. */
	size_t length;

        /* The amount of data the buffer can contain. */
	size_t capacity;
} buffer_t;

/*
 * buffer_init prepares an uninitialized buffer for use.
 */
void buffer_init(buffer_t *b);

/*
 * buffer_alloc creates a new buffer with the specified capacity.
 */
buffer_t *buffer_alloc(size_t capacity);

/*
 * buffer_free frees the buffer and all its contents.
 */
void buffer_free(buffer_t *b);

/*
 * buffer_reset sets the length and its first byte to zero.
 */
void buffer_reset(buffer_t *b);

/*
 * buffer_append appends the |length| bytes at |data| to the buffer.
 */
void buffer_append(buffer_t *b, const void *data, size_t length);

/*
 * buffer_append_str appends |str| to the buffer.
 */
void buffer_append_str(buffer_t *b, const char *str);

#endif  /* BUFFER_H_ */
