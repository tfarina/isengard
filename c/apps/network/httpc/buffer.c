#include "buffer.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"

static NORETURN void fatal(const char *fmt, ...)
{
        va_list args;

        va_start(args, fmt);

        fprintf(stderr, "fatal: ");
        vfprintf(stderr, fmt, args);
        fprintf(stderr, "\n");

        va_end(args);

        exit(EXIT_FAILURE);
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

static void buffer_setlen(buffer_t *b, size_t len)
{
        b->len = len;
        b->data[len] = '\0';
}

buffer_t *buffer_alloc(size_t capacity)
{
        buffer_t *b;

	b = (buffer_t *)malloc(sizeof(buffer_t));

        if (!b) {
	        return NULL;
	}

        b->data = malloc(capacity + 1);
        *b->data = 0; /* always 0 terminate data to allow string functions. */
        b->len = 0;
        b->cap = capacity;

        return b;
}

void buffer_free(buffer_t *b)
{
	free(b->data);
	free(b);
}

static void buffer_grow(buffer_t *b, size_t extra)
{
	size_t want;

        want = b->len + extra;
        if (want > b->cap) {
                b->cap = 2 * want;
                if (b->cap < 64)
                        b->cap = 64;
                b->data = xrealloc(b->data, b->cap);
        }
}

void buffer_append(buffer_t *b, const void *data, size_t len)
{
	buffer_grow(b, len);
	memcpy(b->data + b->len, data, len);
        buffer_setlen(b, b->len + len);
}
