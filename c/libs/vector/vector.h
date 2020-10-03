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
        /* The pointer to the underlying array data. */
        void *data;

        /* The size of each element (in bytes). */
        size_t datasize;

        /* The current number of elements the array can hold. */
	size_t capacity;

        /* The current number of elements stored in the array. */
	size_t size;
} vector_t;

/**
 * Creates a new instance of vector_t with at least the specified capacity.
 *
 * @param[in] datasize  The size of each element (in bytes).
 * @param[in] capacity  The initial capacity (number of elements).
 *
 * @return A new instance of vector_t or `NULL` on error.
 */
vector_t *vector_alloc(size_t datasize, size_t capacity);

/**
 * Inserts the specified element at the end of the array.
 *
 * @param[in,out] self    The vector instance (must not be NULL).
 * @param[in]     element The element to be inserted.
 */
void vector_push_back(vector_t *self, void *element);

/**
 * Returns the pointer to the element at specified location index.
 *
 * @param[in,out] self  The vector instance (must not be NULL).
 * @param[in]     index The index of the element.
 *
 * @return Pointer to the element at specified index or `NULL` if it doesn't exists.
 */
void *vector_at(vector_t const * const self, int unsigned const index);

/**
 * Returns the current number of elements stored in the array.
 *
 * @param[in,out] self The vector instance (must not be NULL).
 *
 * @return The number of elements in the array.
 */
size_t vector_size(vector_t const * const self);

/**
 * Returns the number of elements the container can store without having
 * to expand.
 *
 * @param[in,out] self The vector instance (must not be NULL).
 *
 * @return The number of elements that can be stored before having to expand.
 */
size_t vector_capacity(vector_t const * const self);

/**
 * Returns a pointer to the elements of `self`.
 *
 * @param[in] self pointer to `vector_t`
 * @return pointer to elements, `NULL` if empty.
 */
void *vector_data(vector_t const * const self);

/**
 * Tests whether vector is empty.
 *
 * @param[in,out] self The vector instance.
 * @return non-zero if the vector size is 0, zero otherwise.
 */
int vector_empty(vector_t const * const self);

/**
 * Destroys 'self', freeing all memory that was allocated internally.
 *
 * If 'self' is NULL, no action will be performed.
 *
 * @param[in,out] self The vector instance to be deleted.
 */
void vector_free(vector_t *self);

#ifdef __cplusplus
}
#endif

#endif  /* VECTOR_H_ */
