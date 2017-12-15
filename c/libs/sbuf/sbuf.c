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
        b->len = len;
        b->data[len] = '\0';
}

sbuf *sbuf_create(size_t capacity)
{
        sbuf *b;

	b = calloc(1, sizeof *b);

	b->data = malloc(capacity);
	b->cap = capacity;

        return b;
}

void sbuf_init(sbuf *b)
{
	b->data = NULL;
	b->len = 0;
	b->cap = 0;
}

void sbuf_reset(sbuf *b)
{
	b->len = 0;
}

void sbuf_free(sbuf *b)
{
	free(b->data);
	sbuf_init(b);
}

void sbuf_grow(sbuf *b, size_t extra)
{
	size_t want;

        want = b->len + extra;
        if (b->cap < want) {
                b->cap = 2 * want;
                if (b->cap < 64)
                        b->cap = 64;
                b->data = xrealloc(b->data, b->cap);
        }
}

void sbuf_append(sbuf *b, const void *data, size_t len)
{
	sbuf_grow(b, len);
	memcpy(b->data + b->len, data, len);
        sbuf_setlen(b, b->len + len);
}

void sbuf_append_str(sbuf *b, const char *str)
{
	sbuf_append(b, str, strlen(str));
}
