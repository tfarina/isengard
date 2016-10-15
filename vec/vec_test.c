#include "vec.h"

#include <stdio.h>

int main(void)
{
        Vec files;
        int i;

	vec_init(&files);

        vec_add(&files, "go.tar.gz");
        vec_add(&files, "chromium.zip");

        for (i = 0; i < files.len; i++) {
                printf("%s\n", files.strs[i]);
        }

        vec_free(&files);

        return 0;
}
