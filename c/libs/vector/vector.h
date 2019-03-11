#ifndef VECTOR_H_
#define VECTOR_H_

#include <stddef.h>

/**
 * A vector_t is a variable length array.
 */
typedef struct {
        /* This is a pointer to an array of elements of any type. */
        void **elements; /* void *elements[] */

        /* The max number of allocated elements. */
	size_t capacity;

        /* The current number of elements in the array. */
	size_t size;
} vector_t;

/**
 * Creates a new instance of vector_t with at least the specified capacity.
 *
 * @return A new instance of vector_t or NULL.
 */
vector_t *vector_alloc(int capacity);

/**
 * Inserts the specified element to the back of the array.
 *
 * @param v The vector instance (must not be NULL).
 * @param element The element to be inserted.
 * @return 0 on success, -1 on error.
 */
int vector_append(vector_t *self, void const *element);

/**
 * Returns the current number of elements store in the array.
 *
 * @param v The vector instance (must not be NULL).
 * @return The number of elements in the array.
 */
size_t vector_size(vector_t const * const self);

/**
 * Returns the element at the specified index in the array.
 *
 * @param v The vector instance (must not be NULL).
 * @param index The index of the element.
 * @return The element at specified index.
 */
void *vector_get(vector_t const * const self, int const index);

/**
 * Frees this instance of vector_t.
 * If 'self' is NULL no action will be performed.
 *
 * @param self The vector instance to be deleted.
 */
void vector_free(vector_t *self);

#endif  /* VECTOR_H_ */
