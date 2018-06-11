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
