#ifndef BUFFER_H_
#define BUFFER_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A buffer_t is a generic memory buffer.
 */
typedef struct buffer_s
{
        /* The data contained in the buffer. */
	char *data;

        /* The current length of the buffer, the position for reading/writing. */
	size_t length;

        /* The amount of data the buffer can contain. */
	size_t capacity;
} buffer_t;

/**
 * buffer_alloc creates a new buffer with the specified capacity.
 *
 * @param[in] capacity the size (in) bytes to allocate for the buffer.
 * @return the created buffer.
 */
buffer_t *buffer_alloc(size_t capacity);

/**
 * buffer_reset sets the length and its first byte to zero.
 */
void buffer_reset(buffer_t *b);

/**
 * buffer_write writes |data| to the buffer.
 */
void buffer_write(buffer_t *b, void const *data, size_t length);

/**
 * buffer_free frees the buffer and all its contents.
 */
void buffer_free(buffer_t *b);

#ifdef __cplusplus
}
#endif

#endif  /* BUFFER_H_ */
