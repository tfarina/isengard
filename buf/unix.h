#ifndef UNIX_H_
#define UNIX_H_

#include <stddef.h>

// fatal prints an error message to standard error and exits.
void fatal(const char *msg, ...);

// xstrdup returns a newly allocated copy of p.
// It calls fatal if it runs out of memory.
char* xstrdup(char *p);

// xrealloc grows the allocation p to n bytes and
// returns the new (possibly moved) pointer.
// It calls fatal if it runs out of memory.
void* xrealloc(void *p, size_t n);

// xstrlen returns the length of the NUL-terminated string at |s|.
size_t xstrlen(const char *s);

// xmemmove copies n bytes from src to dst.
void xmemmove(void *dst, const void *src, size_t n);

#endif  // UNIX_H_
