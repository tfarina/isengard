#include "vector.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__GNUC__)
#define NORETURN __attribute__((__noreturn__))
#else
#define NORETURN
#endif

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

static int _vector_add_private(vector_t *v, const void *elem, size_t size, int pos)
{
        void *elemp;

        if (pos < 0 || !v || pos > v->length) {
	        return -1;
	}

        elemp = xmalloc(size);
        memcpy(elemp, elem, size);

        if (v->capacity == v->length) {
                v->entries = xrealloc(v->entries, (v->capacity *= 2) * sizeof(void *));
	}

        memmove(&v->entries[pos + 1], &v->entries[pos], (v->length - pos) * sizeof(void *));

        v->length++;

        v->entries[pos] = elemp;

        return pos;
}

vector_t * vector_alloc(int capacity)
{
        vector_t *v = xcalloc(1, sizeof(vector_t));

        v->entries = xmalloc(capacity * sizeof(void *));
        v->capacity = capacity;

        return v;
}

void vector_free(vector_t **v)
{
        if (v && *v) {
                vector_clear(*v);
                free((*v)->entries);
                free(*v);
        }
}

void vector_clear(vector_t *v)
{
        int i;

        if (v) {
                for (i = 0; i < v->length; i++) {
                        free(v->entries[i]);
                }

                v->length = 0;
        }
}

int vector_append(vector_t *v, const void *elem, size_t size)
{
  return v ? _vector_add_private(v, elem, size, v->length) : -1;
}

int vector_length(const vector_t *v)
{
        return v ? v->length : 0;
}

void *vector_get(const vector_t *v, int pos)
{
        if (pos < 0 || !v || pos >= v->length) {
                return NULL;
        }

        return v->entries[pos];
}
