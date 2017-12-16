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

static void sbuf_setlen(sbuf *b, size_t len)
{
        b->length = len;
        b->data[len] = '\0';
}

sbuf *sbuf_create(size_t capacity)
{
        sbuf *b;

	b = calloc(1, sizeof *b);

	b->data = malloc(capacity);
	b->capacity = capacity;

        return b;
}

void sbuf_destroy(sbuf *b)
{
        free(b->data);
        free(b);
}

void sbuf_init(sbuf *b)
{
	b->data = NULL;
	b->length = 0;
	b->capacity = 0;
}

void sbuf_reset(sbuf *b)
{
	b->length = 0;
}

void sbuf_free(sbuf *b)
{
	free(b->data);
	sbuf_init(b);
}

void sbuf_grow(sbuf *b, size_t extra)
{
	size_t want;

        want = b->length + extra;
        if (b->capacity < want) {
                b->capacity = 2 * want;
                if (b->capacity < 64)
                        b->capacity = 64;
                b->data = xrealloc(b->data, b->capacity);
        }
}

void sbuf_append(sbuf *b, const void *data, size_t len)
{
	sbuf_grow(b, len);
	memcpy(b->data + b->length, data, len);
        sbuf_setlen(b, b->length + len);
}

void sbuf_append_str(sbuf *b, const char *str)
{
	sbuf_append(b, str, strlen(str));
}
