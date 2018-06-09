#include "buffer.h"

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

static void *xrealloc(void *oldptr, size_t newsize)
{
        void *newptr;

	newptr = realloc(oldptr, newsize);
        if (newptr == NULL) {
                fatal("out of memory: %lu", newsize);
	}

	return newptr;
}

static void *xcalloc(size_t nmemb, size_t size)
{
        void *ptr;

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
	size_t want;

        want = b->length + length;
        if (b->capacity < want) {
                b->capacity = 2 * want;
                if (b->capacity < 64)
                        b->capacity = 64;
                b->data = xrealloc(b->data, b->capacity);
        }
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

void buffer_append(buffer_t *b, const void *data, size_t length)
{
	_buffer_realloc(b, length);
	memcpy(b->data + b->length, data, length);
        b->length += length;
        b->data[b->length] = '\0'; /* always 0 terminate data. */
}

void buffer_append_str(buffer_t *b, const char *str)
{
	buffer_append(b, str, strlen(str));
}
