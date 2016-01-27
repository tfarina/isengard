#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
        char cwdbuf[PATH_MAX];

        if (getcwd(cwdbuf, sizeof(cwdbuf)) == NULL) {
                fprintf(stderr, "getcwd() failed\n");
                exit(EXIT_FAILURE);
        }

        printf("%s\n", cwdbuf);

        return 0;
}
