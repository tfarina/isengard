/**
 * @file
 * Implements the vector container class.
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * vector_t is a container that encapsulates a dynamic size array.
 */
typedef struct vector_s
{
        /**
	 * The pointer to the underlying array data.
         */
        void *data;
        /**
         * The size of each element (in bytes).
         */
        size_t datasize;
        /**
         * The current number of elements allocated (nalloc), and
         */
	size_t capacity;
        /**
         * The current number of elements in use (nused) in the array.
         */
	size_t size;
} vector_t;

/* ========================================================================= */
/* Exported functions                                                        */
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/* Constructor and destructor                                                */
/* ------------------------------------------------------------------------- */

/**
 * Creates a new instance of vector_t with at least the specified capacity.
 *
 * @param[in] datasize  The size of each element (in bytes).
 * @param[in] capacity  The initial capacity (number of elements).
 *
 * @return A pointer to a new vector_t or `NULL` on failure.
 */
vector_t *vector_create(size_t datasize, size_t capacity);

/**
 * Destroys the given `vector_t` instance', freeing all memory that was
 * allocated internally.
 *
 * If 'self' is NULL, no action will be performed.
 *
 * @param[in,out] self A pointer to the `vector_t` instance.
 */
void vector_destroy(vector_t *self);

/* ------------------------------------------------------------------------- */
/* Insertion                                                                 */
/* ------------------------------------------------------------------------- */

/**
 * Inserts the specified element at the end of the array.
 *
 * @param[in,out] self    A pointer to the `vector_t` instance (must not be NULL).
 * @param[in]     element The element to be inserted.
 */
void vector_push_back(vector_t *self, void *element);

/* ------------------------------------------------------------------------- */
/* Element access                                                            */
/* ------------------------------------------------------------------------- */

/**
 * Returns the pointer to the element at specified location index.
 *
 * @param[in,out] self   A pointer to the `vector_t` instance (must not be
 *                       NULL).
 * @param[in]     index  The element index.
 *
 * @return Pointer to the element at specified index or `NULL` if it doesn't
 *         exists.
 */
void *vector_at(vector_t const * const self, int unsigned const index);

/**
 * Returns a pointer to the underlying array serving as element storage.
 *
 * @param[in] self A pointer to the `vector_t` instance.
 *
 * @return Pointer to the underlying element storage or `NULL` if it is empty.
 */
void *vector_data(vector_t const * const self);

/* ------------------------------------------------------------------------- */
/* Information                                                               */
/* ------------------------------------------------------------------------- */

/**
 * Returns the current number of elements stored in the array.
 *
 * @param[in,out] self A pointer to the `vector_t` instance (must not be NULL).
 *
 * @return The number of elements in the array.
 */
size_t vector_size(vector_t const * const self);

/**
 * Returns the number of elements the container can store without having
 * to expand.
 *
 * @param[in,out] self A pointer to the `vector_t` instance (must not be NULL).
 *
 * @return The number of elements that can be stored before having to expand.
 */
size_t vector_capacity(vector_t const * const self);

/**
 * Tests whether the vector is empty.
 *
 * @param[in,out] self A pointer to the `vector_t` instance.
 *
 * @return non-zero if the vector size is 0, zero otherwise.
 */
int vector_empty(vector_t const * const self);

/* ------------------------------------------------------------------------- */
/* Deletion                                                                  */
/* ------------------------------------------------------------------------- */

/**
 * Erases all elements from the container.
 *
 * After this call, size() returns zero. Leaves the capacity() of the vector
 * unchanged.
 *
 * @param[in,out] self A pointer to the `vector_t` instance.
 */
void vector_clear(vector_t *self);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* VECTOR_H_ */
