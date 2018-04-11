#include <stdio.h>

#include "sbuf.h"

int main(void) {
        sbuf_t b;
	sbuf_t *bp;

        sbuf_init(&b);

        sbuf_append_str(&b, "C is hard!");

        printf("%s\n", b.data);
        printf("%zu\n", b.length);

	bp = sbuf_alloc(512);

        sbuf_append_str(bp, "C is very hard!");

        printf("%s\n", bp->data);
        printf("%zu\n", bp->length);

	sbuf_free(bp);

        return 0;
}
