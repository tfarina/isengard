#ifndef VECTOR_H_
#define VECTOR_H_

#include <stddef.h>

/*
 * A vector_t is a generic vector.
 */
typedef struct {
        /* The data contained in the buffer. */
	void **items;

        /* The current length of the vector, the position for reading/writing. */
	size_t length;

        /* The amount of elements the vector can contain. */
	size_t capacity;
} vector_t;

vector_t *vector_alloc(int capacity);
void vector_free(vector_t **v);
void vector_clear(vector_t *v);
int vector_append(vector_t *v, const void *item, size_t size);
void *vector_get(const vector_t *v, int pos);
int vector_length(const vector_t *v);

#endif  /* VECTOR_H_ */
