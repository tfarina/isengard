#ifndef UNIX_H_
#define UNIX_H_

#include <stddef.h>

// fatal prints an error message to standard error and exits.
void fatal(char *msg, ...);

// xfree frees the result returned by xmalloc, xstrdup, or xrealloc.
void xfree(void *p);

// xstrdup returns a newly allocated copy of p.
// It calls fatal if it runs out of memory.
char* xstrdup(char *p);

// xrealloc grows the allocation p to n bytes and
// returns the new (possibly moved) pointer.
// It calls fatal if it runs out of memory.
void* xrealloc(void *p, size_t n);

// xstrlen returns the length of the NUL-terminated string at |s|.
size_t xstrlen(const char *s);

// xstrstr returns a pointer to the first occurrence of b in a.
char* xstrstr(char *a, char *b);

// xmemmove copies n bytes from src to dst.
void xmemmove(void *dst, const void *src, size_t n);

// xmemcmp compares the n-byte regions starting at a and at b.
int xmemcmp(void *a, void *b, int n);

#endif  // UNIX_H_
