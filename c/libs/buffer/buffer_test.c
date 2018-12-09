#include <stdio.h>
#include <string.h>

#include "buffer.h"

int main(void) {
        buffer_t *b;
	buffer_t *bp;
        char *message1;
        char *message2;

        b = buffer_alloc(16);

        message1 = "C is hard!";
        buffer_write(b, message1, strlen(message1));

        printf("%s\n", b->data);
        printf("%zu\n", b->length);

	bp = buffer_alloc(512);

        message2 = "C is very hard";
        buffer_write(bp, message2, strlen(message2));

        printf("%s\n", bp->data);
        printf("%zu\n", bp->length);

	buffer_free(b);
	buffer_free(bp);

        return 0;
}
