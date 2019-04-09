#include "vector.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"

static void NORETURN fatal(const char *msg, ...)
{
        va_list args;

        va_start(args, msg);

        fprintf(stderr, "fatal: ");
        vfprintf(stderr, msg, args);
        fprintf(stderr, "\n");

        va_end(args);

        exit(EXIT_FAILURE);
}

static void *xmalloc(size_t size)
{
        void *ptr;

        if (size == 0) {
                fatal("zero size");
	}

        ptr = malloc(size);
        if (ptr == NULL) {
	        fatal("Out of memory, malloc failed, tried to allocate %lu bytes", (unsigned long)size);
        }

        return ptr;
}

static void *xcalloc(size_t nmemb, size_t size)
{
        void *ptr;

        ptr = calloc(nmemb, size);
        if (ptr == NULL) {
	        fatal("xcalloc failed");
	}

        return ptr;
}

static void *xrealloc(void *oldptr, size_t newsize)
{
        void *newptr;

	newptr = realloc(oldptr, newsize);
        if (newptr == NULL) {
                fatal("out of memory: %lu", newsize);
	}

	return newptr;
}

static int _vector_insert(vector_t *self, int index, void *element)
{
        if (index < 0 || !self || index > self->size) {
	        return -1;
	}

        if (self->size + 1 > self->capacity) {
                size_t const new_capacity = (self->size + 1) * 2;
                self->elements = xrealloc(self->elements, new_capacity * sizeof(void *));
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

        self = xcalloc(1, sizeof(vector_t));

        self->elements = xmalloc(capacity * sizeof(void *));
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

void *vector_get(vector_t const * const self, int unsigned const index)
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
