#include "vector.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *
 * @return 0       If no error.
 * @return -EINVAL If invalid argument.
 * @return -ENOMEM If out of memory.
 */
static int _vector_insert(vector_t *self, int index, void *element)
{
        size_t element_size;
        void **temp;

        if (self == NULL) {
	        return -EINVAL;
	}

        if (index < 0 || index > self->size) {
	        return -EINVAL;
	}

        element_size = sizeof(void *);

        if (self->size + 1 > self->capacity) {
                /* Triple the capacity, then divide by half and finally increase by one unit. */
                size_t const new_capacity = self->capacity * 3 / 2 + 1;
                temp = realloc(self->data, new_capacity * element_size);
                if (temp == NULL) {
                        return -ENOMEM;
                }

                self->data = temp;
                self->capacity = new_capacity;
	}

        memmove(self->data + index + 1, self->data + index,
                (self->size - index) * element_size);

        self->data[index] = element;
        self->size++;

        return 0;
}

vector_t *vector_alloc(int capacity)
{
        vector_t *self;
        size_t element_size;

        self = (vector_t *) malloc(sizeof(*self));
        if (self == NULL) {
	        return NULL;
	}

	element_size = sizeof(void *);

        self->data = (void **) malloc(capacity * element_size);
        if (self->data == NULL) {
	        free(self);
	        return NULL;
	}

        self->capacity = capacity;
        self->size = 0;

        return self;
}

void vector_push_back(vector_t *self, void *element)
{
        if (self == NULL) {
                return;
        }

        _vector_insert(self, self->size, element);
}

void *vector_at(vector_t const * const self, int unsigned const index)
{
        if (self == NULL) {
                return NULL;
        }

        if (index >= self->size) {
                return NULL;
        }

        return self->data[index];
}

size_t vector_size(vector_t const * const self)
{
        if (self == NULL) {
	        return 0;
	}

        return self->size;
}

size_t vector_capacity(vector_t const * const self)
{
        if (self == NULL) {
	        return 0;
	}

        return self->capacity;
}

void *vector_data(vector_t const * const self)
{
        if (self == NULL) {
                return NULL;
        }

        return *self->data;
}

int vector_empty(vector_t const * const self)
{
        if (self == NULL) {
                return 1;
        }

        return self->size == 0;
}

void vector_free(vector_t *self)
{
        if (self == NULL) {
                return;
        }

        free(self->data);
        self->data = NULL;
        self->capacity = 0;
        self->size = 0;
        free(self);
}
