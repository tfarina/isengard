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

static int _vector_add_private(vector_t *v, void const *elem, int index)
{
        void *elemp;

        if (index < 0 || !v || index > v->size) {
	        return -1;
	}

        elemp = (void *)elem;

        if (v->capacity == v->size) {
                v->elements = xrealloc(v->elements, (v->capacity *= 2) * sizeof(void *));
	}

        memmove(&v->elements[index + 1], &v->elements[index], (v->size - index) * sizeof(void *));

        v->size++;

        v->elements[index] = elemp;

        return index;
}

vector_t * vector_alloc(int capacity)
{
        vector_t *v = xcalloc(1, sizeof(vector_t));

        v->elements = xmalloc(capacity * sizeof(void *));
        v->capacity = capacity;

        return v;
}

void vector_free(vector_t **v)
{
        if (v && *v) {
                /*vector_clear(*v);*/
                free((*v)->elements);
                (*v)->elements = NULL;
                (*v)->capacity = 0;
                (*v)->size = 0;
                free(*v);
        }
}

void vector_clear(vector_t *v)
{
        int i;

        if (v) {
                for (i = 0; i < v->size; i++) {
                        free(v->elements[i]);
                }

                v->size = 0;
        }
}

int vector_append(vector_t *v, void const *element)
{
        return v ? _vector_add_private(v, element, v->size) : -1;
}

size_t vector_size(vector_t const *v)
{
        return v ? v->size : 0;
}

void *vector_get(vector_t const *v, int index)
{
        if (index < 0 || !v || index >= v->size) {
                return NULL;
        }

        return v->elements[index];
}
