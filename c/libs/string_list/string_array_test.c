#include "string_array.h"

#include <stdio.h>

int main(void)
{
        string_array_t files;
        int i;

	string_array_init(&files);

        string_array_add(&files, "go.tar.gz");
        string_array_add(&files, "chromium.zip");

        for (i = 0; i < files.len; i++) {
                printf("%s\n", files.strs[i]);
        }

        string_array_free(&files);

        return 0;
}
