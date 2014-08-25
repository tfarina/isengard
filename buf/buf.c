#include "buf.h"

#include "unix.h"

void buf_init(Buf *b)
{
	b->data = nil;
	b->len = 0;
	b->cap = 0;
}

void buf_reset(Buf *b)
{
	b->len = 0;
}

void buf_free(Buf *b)
{
	xfree(b->data);
	buf_init(b);
}

void buf_grow(Buf *b, int n)
{
	int want;

        want = b->len + n;
        if (want > b->cap) {
                b->cap = 2 * want;
                if (b->cap < 64)
                        b->cap = 64;
                b->data = xrealloc(b->data, b->cap);
        }
}

// buf_write appends the |len| bytes at |data| to the buffer.
static void buf_write(Buf *b, void *data, int len)
{
	buf_grow(b, len);
	xmemmove(b->data + b->len, data, len);
	b->len += len;
}

void buf_write_str(Buf *b, char *string)
{
	buf_write(b, string, xstrlen(string));
}
