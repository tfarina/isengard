#include "unix.h"

#include <stdlib.h>
#include <string.h>

void fatal(const char *msg, ...)
{
}

char* xstrdup(char *p)
{
	p = strdup(p);
        if (p == NULL)
                fatal("out of memory");
	return p;
}

void* xrealloc(void *p, size_t n)
{
	p = realloc(p, n);
        if (p == NULL)
                fatal("out of memory");
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
