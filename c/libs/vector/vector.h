#ifndef VECTOR_H_
#define VECTOR_H_

#include <stddef.h>

/*
 * A vector_t is a generic vector.
 */
typedef struct {
        /* This is a pointer to an array of elements of any type. */
        void **entries; /* void *entries[] */

        /* The current number of entries in the vector. */
	size_t length;

        /* The number of allocated entries. */
	size_t capacity;
} vector_t;

vector_t *vector_alloc(int capacity);
void vector_free(vector_t **v);
void vector_clear(vector_t *v);
int vector_append(vector_t *v, void const *item);
size_t vector_length(vector_t const *v);
void *vector_get(vector_t const *v, int pos);

#endif  /* VECTOR_H_ */
