#include <stdio.h>

#include "buf.h"

int main(void) {
        Buf b;

        buf_init(&b);

        buf_write_str(&b, "Hi, C is complicated, isn't it?");

        printf("%s\n", b.data);
        printf("%d\n", b.len);

        buf_free(&b);

        return 0;
}
