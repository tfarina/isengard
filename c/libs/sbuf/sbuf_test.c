#include <stdio.h>

#include "sbuf.h"

int main(void) {
        sbuf b;

        sbuf_init(&b);

        sbuf_append_str(&b, "C is hard!");

        printf("%s\n", b.data);
        printf("%zu\n", b.len);

        sbuf_free(&b);

        return 0;
}
