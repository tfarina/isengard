#include "buffer.h"

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
                fatal("xmalloc: zero size");
	}

        if ((ptr = malloc(size)) == NULL) {
	        fatal("xmalloc failed");
        }

        return ptr;
}

static void *xrealloc(void *oldptr, size_t newsize)
{
        void *newptr;

        if ((newptr = realloc(oldptr, newsize)) == NULL) {
                fatal("xrealloc failed");
	}

	return newptr;
}

static void *xcalloc(size_t nmemb, size_t size)
{
        void *ptr;

	if (size == 0 || nmemb == 0) {
                fatal("xcalloc: zero size");
	}

        if ((ptr = calloc(nmemb, size)) == NULL) {
	        fatal("xcalloc failed");
	}

        return ptr;
}

/*
 * _buffer_realloc ensures that the buffer has at least |length| more bytes between its
 * length and capacity.
 */
static void _buffer_realloc(buffer_t *b, size_t length)
{
        b->capacity = length;
        if (b->capacity < 64)
	        b->capacity = 64;
        b->data = xrealloc(b->data, b->capacity);
}

void buffer_init(buffer_t *b)
{
	b->data = NULL;
	b->length = 0;
	b->capacity = 0;
}

buffer_t *buffer_alloc(size_t capacity)
{
        buffer_t *b;

	b = xcalloc(1, sizeof *b);

	b->data = xmalloc(capacity);
	b->capacity = capacity;

	b->length = 0;

        return b;
}

void buffer_free(buffer_t *b)
{
        free(b->data);
        free(b);
}

void buffer_reset(buffer_t *b)
{
	b->length = 0;
	*b->data = 0;
}

void buffer_write(buffer_t *b, const void *data, size_t length)
{
        if (b->capacity < b->length + length) {
          _buffer_realloc(b, b->length * 2 + length);
	}
	memcpy(b->data + b->length, data, length);
        b->length += length;
        b->data[b->length] = '\0'; /* always 0 terminate data. */
}
