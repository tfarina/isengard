#include <stdio.h>

#include "buf.h"

int main(void) {
        Buf b;

        buf_init(&b);

        printf("%d\n", b.len);

        buf_free(&b);

        return 0;
}
