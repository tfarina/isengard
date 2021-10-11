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
         * The pointer to the underlying data.
         */
	char *data;
        /**
	 * The number of bytes currently allocated.
         */
        size_t capacity;
        /**
         * The current number of characters (bytes) in the buffer.
         */
	size_t size;
} buffer_t;

/* ------------------------------------------------------------------------- */
/* Constructor and destructor                                                */
/* ------------------------------------------------------------------------- */

/**
 * Creates a new instance of buffer_t with at least the specified capacity.
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
 *
 * @param[in,out] self A pointer to the `buffer_t` instance.
 */
void buffer_clear(buffer_t *self);

/* ------------------------------------------------------------------------- */
/* Insertion                                                                 */
/* ------------------------------------------------------------------------- */

/**
 * buffer_write writes |data| to the end of the buffer.
 *
 * @param[in,out] self A pointer to the `buffer_t` instance.
 */
void buffer_write(buffer_t *self, void const *data, size_t size);

/* ------------------------------------------------------------------------- */
/* Element access                                                            */
/* ------------------------------------------------------------------------- */

/**
 * Returns a pointer to the underlying data.
 *
 * @param[in] self A pointer to the `buffer_t` instance.
 *
 * @return Pointer to the underlying data or `NULL` if it is empty.
 */
char *buffer_data(buffer_t const * const self);

/* ------------------------------------------------------------------------- */
/* Information                                                               */
/* ------------------------------------------------------------------------- */

/**
 * Returns the current number of characters (bytes) stored in the array.
 *
 * @param[in,out] self A pointer to the `buffer_t` instance (must not be NULL).
 *
 * @return The number of characters (bytes) in the array.
 */
size_t buffer_size(buffer_t const * const self);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* BUFFER_H_ */
