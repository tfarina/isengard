#include <stdio.h>

#include "sbuf.h"

int main(void) {
        sbuf b;
	sbuf *bp;

        sbuf_init(&b);

        sbuf_append_str(&b, "C is hard!");

        printf("%s\n", b.data);
        printf("%zu\n", b.length);

        sbuf_free(&b);

	bp = sbuf_create(512);
        sbuf_append_str(bp, "C is very hard!");
        printf("%s\n", bp->data);
	sbuf_free(bp);

        return 0;
}
