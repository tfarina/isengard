#include "string_list.h"

#include <stdio.h>

int main(void)
{
        string_list_t files;
        int i;

	string_list_init(&files);

        string_list_add(&files, "go.tar.gz");
        string_list_add(&files, "chromium.zip");

        for (i = 0; i < files.len; i++) {
                printf("%s\n", files.strs[i]);
        }

        string_list_free(&files);

        return 0;
}
