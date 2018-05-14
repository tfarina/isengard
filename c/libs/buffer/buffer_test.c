#include <stdio.h>

#include "buffer.h"

int main(void) {
        buffer_t b;
	buffer_t *bp;

        buffer_init(&b);

        buffer_append_str(&b, "C is hard!");

        printf("%s\n", b.data);
        printf("%zu\n", b.length);

	bp = buffer_alloc(512);

        buffer_append_str(bp, "C is very hard!");

        printf("%s\n", bp->data);
        printf("%zu\n", bp->length);

	buffer_free(bp);

        return 0;
}
