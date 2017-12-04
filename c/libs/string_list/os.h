#ifndef OS_H_
#define OS_H_

#include <stddef.h>

// xrealloc grows the allocation p to n bytes and
// returns the new (possibly moved) pointer.
// It calls fatal if it runs out of memory.
void *xrealloc(void *oldptr, size_t size);

// xstrdup returns a newly allocated copy of s.
// It calls fatal if it runs out of memory.
char *xstrdup(char *s);

#endif  // OS_H_
