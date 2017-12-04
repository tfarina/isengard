#include "os.h"

#include <stdlib.h>
#include <string.h>

#include "log.h"

void* xrealloc(void *oldptr, size_t size)
{
        void *newptr;

	if ((newptr = realloc(oldptr, size)) == NULL)
                fatal("out of memory");

	return newptr;
}

char* xstrdup(char *s)
{
	s = strdup(s);
        if (s == NULL)
                fatal("out of memory");
	return s;
}
