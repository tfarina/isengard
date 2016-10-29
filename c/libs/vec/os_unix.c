#include "os.h"

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
        if (p == NULL)
                fatal("out of memory");
	return p;
}

void* xrealloc(void *p, int n)
{
	p = realloc(p, n);
        if (p == NULL)
                fatal("out of memory");
	return p;
}
