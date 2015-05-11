#ifndef UNIX_H_
#define UNIX_H_

#include <stddef.h>

// xrealloc grows the allocation p to n bytes and
// returns the new (possibly moved) pointer.
// It calls fatal if it runs out of memory.
void* xrealloc(void *p, size_t n);

// xmemmove copies n bytes from src to dst.
void xmemmove(void *dst, const void *src, size_t n);

#endif  // UNIX_H_
