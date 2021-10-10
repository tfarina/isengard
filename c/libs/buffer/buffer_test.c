#include <stdio.h>
#include <string.h>

#include "buffer.h"

int main(void) {
        buffer_t *b;
	buffer_t *bp;
        char *message1;
        char *message2;

        b = buffer_create(16);

        message1 = "C is hard!";
        buffer_write(b, message1, strlen(message1));

        printf("%s\n", buffer_data(b));
        printf("%zu\n", buffer_size(b));

	bp = buffer_create(512);

        message2 = "C is very hard";
        buffer_write(bp, message2, strlen(message2));

        printf("%s\n", buffer_data(bp));
        printf("%zu\n", buffer_size(bp));

	buffer_destroy(b);
	buffer_destroy(bp);

        return 0;
}
