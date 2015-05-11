#include "unix.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

void* xrealloc(void *p, size_t n)
{
	p = realloc(p, n);
        if (p == NULL)
                malloc_fail(n);
	return p;
}

size_t xstrlen(const char *s)
{
	return strlen(s);
}

void xmemmove(void *dst, const void *src, size_t n)
{
	memmove(dst, src, n);
}
