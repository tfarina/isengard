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
        /**
         * The data contained in the buffer.
         */
	char *data;
        /**
         * The current number of characters (bytes) in the buffer.
         */
	size_t size;
        /**
	 * The number of bytes currently allocated.
         */
	size_t capacity;
} buffer_t;

/* ------------------------------------------------------------------------- */
/* Constructor and destructor                                                */
/* ------------------------------------------------------------------------- */

/**
 * buffer_create creates a new buffer with the specified capacity.
 *
 * @param[in] capacity the size (in) bytes to allocate for the buffer.
 *
 * @return A pointer to a new buffer_t or `NULL` on failure.
 */
buffer_t *buffer_create(size_t capacity);

/**
 * Destroys the given `buffer_t` instance', freeing all memory that was
 * allocated internally.
 *
 * If 'self' is NULL, no action will be performed.
 *
 * @param[in,out] self A pointer to the `buffer_t` instance.
 */
void buffer_destroy(buffer_t *self);

/* ------------------------------------------------------------------------- */
/* Deletion                                                                  */
/* ------------------------------------------------------------------------- */

/**
 * buffer_clear sets the length and its first byte to zero.
 */
void buffer_clear(buffer_t *self);

/* ------------------------------------------------------------------------- */
/* Insertion                                                                 */
/* ------------------------------------------------------------------------- */

/**
 * buffer_write writes |data| to the end of the buffer.
 */
void buffer_write(buffer_t *self, void const *data, size_t size);

#ifdef __cplusplus
}
#endif

#endif  /* BUFFER_H_ */
