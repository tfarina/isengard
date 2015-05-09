#include <stdio.h>

#include "sbuf.h"

int main(void) {
        sbuf b;

        sbuf_init(&b);

        sbuf_write_str(&b, "Hi, C is complicated, isn't it?");

        printf("%s\n", b.data);
        printf("%zu\n", b.len);

        sbuf_free(&b);

        return 0;
}
