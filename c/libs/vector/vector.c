#include "vector.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Inserts the specified element at the given position at #self.
 *
 * @param self The container instance.
 * @param index The position where the element has to be inserted.
 * @param element The element to be inserted.
 *
 * @return On success, zero is returned. On error, -1 is returned.
 */
static int _vector_insert(vector_t *self, int index, void *element)
{
        if (self == NULL) {
	        return -1;
	}

        if (index < 0 || index > self->size) {
	        return -1;
	}

        if (self->size + 1 > self->capacity) {
                /* Increase the capacity by one to make room for the new element, then double it. */
	        size_t const new_capacity = (self->capacity + 1) * 2;
                self->data = realloc(self->data, self->datasize * new_capacity);
                if (self->data == NULL) {
                        return -1;
                }

                self->capacity = new_capacity;
	}

        memcpy((char *)self->data + (index * self->datasize), element, self->datasize);
        self->size++;

        return 0;
}

vector_t *vector_alloc(size_t datasize, size_t capacity)
{
        vector_t *self;

        self = (vector_t *) malloc(sizeof(*self));
        if (self == NULL) {
	        return NULL;
	}

        self->data = malloc(datasize * capacity);
        if (self->data == NULL) {
	        free(self);
	        return NULL;
	}

	self->datasize = datasize;
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

        /**
         * A cast to char* is necessary as void* cannot deal with pointer arithmetic.
         */
        return (void *) ((char *)self->data + (index * self->datasize));
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

        return self->data;
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

	if (self->data != NULL) {
                free(self->data);
                self->data = NULL;
	}

        self->capacity = 0;
        self->size = 0;

        free(self);
}
