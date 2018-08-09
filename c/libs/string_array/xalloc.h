#ifndef XALLOC_H_
#define XALLOC_H_

#include <stddef.h>

/**
 * xrealloc grows the allocation p to n bytes and
 * returns the new (possibly moved) pointer.
 * It calls fatal if it runs out of memory.
 */
void *xrealloc(void *oldptr, size_t newsize);

/**
 * xstrdup returns a newly allocated copy of s.
 * It calls fatal if it runs out of memory.
 */
char *xstrdup(char *s);

#endif  /* XALLOC_H_ */
