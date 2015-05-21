#include "sbuf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__GNUC__)
#define NORETURN        __attribute__((__noreturn__))
#else
#define NORETURN
#endif

static void NORETURN malloc_fail(unsigned long size)
{
        fprintf(stderr, "out of memory: %lu\n", size);
        abort();
}

static void* xrealloc(void *p, size_t n)
{
	p = realloc(p, n);
        if (p == NULL)
                malloc_fail(n);
	return p;
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

void sbuf_grow(sbuf *b, int n)
{
	int want;

        want = b->len + n;
        if (want > b->cap) {
                b->cap = 2 * want;
                if (b->cap < 64)
                        b->cap = 64;
                b->data = xrealloc(b->data, b->cap);
        }
}

static void sbuf_setlen(sbuf *b, size_t len)
{
        b->len = len;
        b->data[len] = '\0';
}

void sbuf_write(sbuf *b, const void *data, size_t len)
{
	sbuf_grow(b, len);
	memcpy(b->data + b->len, data, len);
        sbuf_setlen(b, b->len + len);
}

void sbuf_write_str(sbuf *b, const void *str)
{
	sbuf_write(b, str, strlen(str));
}
