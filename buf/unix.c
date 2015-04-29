#include "unix.h"

#include <stdlib.h>
#include <string.h>

void fatal(char *msg, ...)
{
}

void xfree(void* p)
{
        free(p);
}

char* xstrdup(char *p)
{
	p = strdup(p);
        if (p == nil)
                fatal("out of memory");
	return p;
}

void* xrealloc(void *p, size_t n)
{
	p = realloc(p, n);
        if (p == nil)
                fatal("out of memory");
	return p;
}

size_t xstrlen(const char *s)
{
	return strlen(s);
}

char* xstrstr(char *a, char *b)
{
	return strstr(a, b);
}

void xmemmove(void *dst, const void *src, size_t n)
{
	memmove(dst, src, n);
}

int xmemcmp(void *a, void *b, int n)
{
	return memcmp(a, b, n);
}
