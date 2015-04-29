#ifndef BUF_H_
#define BUF_H_

// A sbuf is a byte buffer.
typedef struct {
	char *data;
	int len;
	int cap;
} sbuf;

// buf_init prepares an uninitialized buffer for use.
void buf_init(sbuf *b);

// buf_reset truncates the buffer back to zero length.
void buf_reset(sbuf *b);

// buf_free frees the storage associated with a buffer.
void buf_free(sbuf *b);

// buf_grow ensures that the buffer has at least n more bytes between its
// len and cap.
void buf_grow(sbuf *b, int n);

// buf_write_str appends |str| to the buffer.
void buf_write_str(sbuf *b, const void *str);

#endif  // BUF_H_
