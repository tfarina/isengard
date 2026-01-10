#include "vector.h"

#include <stdlib.h>
#include <string.h>

#define VECTOR_DEFAULT_CAPACITY 4

vector_t *vector_create(size_t datasize, size_t capacity)
{
        vector_t *self;

        self = malloc(sizeof(vector_t));
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

void vector_destroy(vector_t *self)
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

int vector_push_back(vector_t *self, void *element)
{
        if (self == NULL) {
	        return -1;
	}

        if (self->size == self->capacity) {
                size_t new_capacity = self->capacity == 0 ? VECTOR_DEFAULT_CAPACITY : self->capacity * 2;
                void *new_data = realloc(self->data, new_capacity * self->datasize);
                if (new_data == NULL) {
		        return -1; /* self->data unchanged */
                }

                self->data = new_data;
                self->capacity = new_capacity;
	}

        memcpy((char *)self->data + (self->size * self->datasize), element, self->datasize);
        self->size++;

	return 0;
}

void *vector_at(vector_t const * const self, int unsigned const index)
{
        if (self == NULL) {
                return NULL;
        }

        if (index >= self->size) {
                return NULL;
        }

        /*
         * A cast to char* is necessary as void* cannot deal with pointer arithmetic.
         */
        return (char *)self->data + (index * self->datasize);
}

void *vector_data(vector_t const * const self)
{
        if (self == NULL) {
                return NULL;
        }

        return self->data;
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

int vector_empty(vector_t const * const self)
{
        if (self == NULL) {
                return 1;
        }

        return self->size == 0;
}

void vector_clear(vector_t *self)
{
        if (self == NULL) {
                return;
        }

        self->size = 0;
}
