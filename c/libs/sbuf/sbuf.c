#include "sbuf.h"

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

static void *xrealloc(void *oldptr, size_t newsize)
{
        void *newptr;

	newptr = realloc(oldptr, newsize);
        if (newptr == NULL)
                fatal("out of memory: %lu", newsize);

	return newptr;
}

/*
 * _sbuf_realloc ensures that the buffer has at least |length| more bytes between its
 * length and capacity.
 */
static void _sbuf_realloc(sbuf_t *b, size_t length)
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

void sbuf_init(sbuf_t *b)
{
	b->data = NULL;
	b->length = 0;
	b->capacity = 0;
}

sbuf_t *sbuf_create(size_t capacity)
{
        sbuf_t *b;

	b = calloc(1, sizeof *b);

	b->data = malloc(capacity);
	b->capacity = capacity;

	b->length = 0;

        return b;
}

void sbuf_free(sbuf_t *b)
{
        free(b->data);
        free(b);
}

void sbuf_reset(sbuf_t *b)
{
	b->length = 0;
	*b->data = 0;
}

void sbuf_append(sbuf_t *b, const void *data, size_t length)
{
	_sbuf_realloc(b, length);
	memcpy(b->data + b->length, data, length);
        b->length += length;
        b->data[b->length] = '\0'; /* always 0 terminate data. */
}

void sbuf_append_str(sbuf_t *b, const char *str)
{
	sbuf_append(b, str, strlen(str));
}
