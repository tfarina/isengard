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
        void **temp;

        if (self == NULL) {
	        return -EINVAL;
	}

        if (index < 0 || index > self->size) {
	        return -EINVAL;
	}

        if (self->size + 1 > self->capacity) {
                /* Triple the capacity, then divide by half and finally increase by one unit. */
                size_t const new_capacity = self->capacity * 3 / 2 + 1;
                temp = realloc(self->elements, new_capacity * sizeof(void *));
                if (temp == NULL) {
                        return -ENOMEM;
                }

                self->elements = temp;
                self->capacity = new_capacity;
	}

        memmove(&self->elements[index + 1], &self->elements[index], (self->size - index) * sizeof(void *));

        self->elements[index] = element;
        self->size++;

        return 0;
}

vector_t *vector_alloc(int capacity)
{
        vector_t *self = NULL;
        void **temp;

        self = calloc(1, sizeof(vector_t));
        if (self == NULL) {
	        return NULL;
	}

        temp = malloc(capacity * sizeof(void *));
        if (temp == NULL) {
	        free(self);
	        return NULL;
	}

        self->elements = temp;
        self->capacity = capacity;

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

        return self->elements[index];
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

void vector_free(vector_t *self)
{
        if (self == NULL) {
                return;
        }

        free(self->elements);
        self->elements = NULL;
        self->capacity = 0;
        self->size = 0;
        free(self);
}
