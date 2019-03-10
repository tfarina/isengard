#ifndef VECTOR_H_
#define VECTOR_H_

#include <stddef.h>

/**
 * A vector_t is a variable length array.
 */
typedef struct {
        /* This is a pointer to an array of elements of any type. */
        void **entries; /* void *entries[] */

        /* The max number of allocated entries. */
	size_t capacity;

        /* The current number of entries in the array. */
	size_t size;
} vector_t;

vector_t *vector_alloc(int capacity);
void vector_free(vector_t **v);
void vector_clear(vector_t *v);
int vector_append(vector_t *v, void const *item);
size_t vector_size(vector_t const *v);
void *vector_get(vector_t const *v, int index);

#endif  /* VECTOR_H_ */
