#ifndef BUF_H_
#define BUF_H_

// A Buf is a byte buffer.
typedef struct Buf Buf;
struct Buf {
	char *data;
	int len;
	int cap;
};

// buf_init prepares an uninitialized buffer for use.
void buf_init(Buf *b);

// buf_reset truncates the buffer back to zero length.
void buf_reset(Buf *b);

// buf_free frees the storage associated with a buffer.
void buf_free(Buf *b);

// buf_grow ensures that the buffer has at least n more bytes between its
// len and cap.
void buf_grow(Buf *b, int n);

// buf_write_str appends |string| to the buffer.
void buf_write_str(Buf *b, char *string);

#endif  // BUF_H_
