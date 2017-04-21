#ifndef SBUF_H_
#define SBUF_H_

#include <stddef.h>

/*
 * A sbuf is a byte buffer.
 */
typedef struct {
        /* The data contained in the buffer. */
	char *data;

        /* The current length of the buffer, the position for reading/writing. */
	size_t len;

        /* The amount of data the buffer can contain. */
	size_t cap;
} sbuf;

/*
 * sbuf_init prepares an uninitialized buffer for use.
 */
void sbuf_init(sbuf *b);

/*
 * sbuf_reset truncates the buffer back to zero length.
 */
void sbuf_reset(sbuf *b);

/*
 * sbuf_free frees the storage associated with a buffer.
 */
void sbuf_free(sbuf *b);

/*
 * sbuf_grow ensures that the buffer has at least |extra| more bytes between its
 * len and cap.
 */
void sbuf_grow(sbuf *b, size_t extra);

/*
 * sbuf_append appends the |len| bytes at |data| to the buffer.
 */
void sbuf_append(sbuf *b, const void *data, size_t len);

/*
 * sbuf_append_str appends |str| to the buffer.
 */
void sbuf_append_str(sbuf *b, const char *str);

#endif  /* SBUF_H_ */
