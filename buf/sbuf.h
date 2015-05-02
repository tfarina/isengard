#ifndef SBUF_H_
#define SBUF_H_

#include <stddef.h>

// A sbuf is a byte buffer.
typedef struct {
	char *data;
	int len;
	int cap;
} sbuf;

// sbuf_init prepares an uninitialized buffer for use.
void sbuf_init(sbuf *b);

// sbuf_reset truncates the buffer back to zero length.
void sbuf_reset(sbuf *b);

// sbuf_free frees the storage associated with a buffer.
void sbuf_free(sbuf *b);

// sbuf_grow ensures that the buffer has at least n more bytes between its
// len and cap.
void sbuf_grow(sbuf *b, int n);

// sbuf_write appends the |len| bytes at |data| to the buffer.
void sbuf_write(sbuf *b, const void *data, size_t len);

// sbuf_write_str appends |str| to the buffer.
void sbuf_write_str(sbuf *b, const void *str);

#endif  // SBUF_H_
