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

        if ((ptr = malloc(size)) == NULL) {
	  fatal("Out of memory, malloc failed, tried to allocate %lu bytes", (unsigned long)size);
        }

        return ptr;
}

static void *xcalloc(size_t nmemb, size_t size)
{
        void *ptr;

        if ((ptr = calloc(nmemb, size)) == NULL) {
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

static int _vector_insert(vector_t *v, int index, void *element)
{
        if (index < 0 || !v || index > v->size) {
	        return -1;
	}

        if (v->capacity == v->size) {
                v->elements = xrealloc(v->elements, (v->capacity *= 2) * sizeof(void *));
	}

        memmove(&v->elements[index + 1], &v->elements[index], (v->size - index) * sizeof(void *));

        v->size++;

        v->elements[index] = element;

        return index;
}

vector_t *vector_alloc(int capacity)
{
        vector_t *self = NULL;

        self = xcalloc(1, sizeof(vector_t));

        self->elements = xmalloc(capacity * sizeof(void *));
        self->capacity = capacity;

        return self;
}

int vector_push_back(vector_t *self, void *element)
{
        return self ? _vector_insert(self, self->size, element) : -1;
}

void *vector_get(vector_t const * const self, int const index)
{
        if (index < 0 || !self || index >= self->size) {
                return NULL;
        }

        return self->elements[index];
}

size_t vector_size(vector_t const * const self)
{
        return self ? self->size : 0;
}

void vector_free(vector_t *self)
{
        if (self) {
                free(self->elements);
                self->elements = NULL;
                self->capacity = 0;
                self->size = 0;
                free(self);
        }
}
