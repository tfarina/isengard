#ifndef VECTOR_H_
#define VECTOR_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

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
 * @param self The vector instance (must not be NULL).
 * @param element The element to be inserted.
 */
void vector_push_back(vector_t *self, void *element);

/**
 * Returns the element at the specified index in the array.
 *
 * @param self The vector instance (must not be NULL).
 * @param index The index of the element.
 * @return The element at specified index.
 */
void *vector_get(vector_t const * const self, int unsigned const index);

/**
 * Returns the current number of elements store in the array.
 *
 * @param self The vector instance (must not be NULL).
 * @return The number of elements in the array.
 */
size_t vector_size(vector_t const * const self);

/**
 * Returns the number of elements the container can store without having
 * to expand.
 *
 * @param self The vector instance (must not be NULL).
 * @return The number of elements that can be stored before having to expand.
 */
size_t vector_capacity(vector_t const * const self);

/**
 * Frees this instance of vector_t.
 * If 'self' is NULL no action will be performed.
 *
 * @param self The vector instance to be deleted.
 */
void vector_free(vector_t *self);

#ifdef __cplusplus
}
#endif

#endif  /* VECTOR_H_ */
