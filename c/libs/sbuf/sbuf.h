#ifndef SBUF_H_
#define SBUF_H_

#include <stddef.h>

/*
 * A sbuf_t is a generic memory buffer.
 */
typedef struct {
        /* The data contained in the buffer. */
	char *data;

        /* The current length of the buffer, the position for reading/writing. */
	size_t length;

        /* The amount of data the buffer can contain. */
	size_t capacity;
} sbuf_t;

/*
 * sbuf_init prepares an uninitialized buffer for use.
 */
void sbuf_init(sbuf_t *b);

/*
 * sbuf_create creates a new buffer with the specified capacity.
 */
sbuf_t *sbuf_create(size_t capacity);

/*
 * sbuf_free frees the buffer and all its contents.
 */
void sbuf_free(sbuf_t *b);

/*
 * sbuf_reset sets the length and its first byte to zero.
 */
void sbuf_reset(sbuf_t *b);

/*
 * sbuf_append appends the |len| bytes at |data| to the buffer.
 */
void sbuf_append(sbuf_t *b, const void *data, size_t len);

/*
 * sbuf_append_str appends |str| to the buffer.
 */
void sbuf_append_str(sbuf_t *b, const char *str);

#endif  /* SBUF_H_ */
