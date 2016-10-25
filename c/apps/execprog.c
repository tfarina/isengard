#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void
print_java_version(void)
{
        char *cmd[] = { "java", "-version", (char *)0 };
        execv("/usr/bin/java", cmd);

        /* If we reach here, an error has occurred. */
        fprintf(stderr, "execv /usr/bin/java failed: %s\n", strerror(errno));
}

int main(void) {
        print_java_version();

        return 0;
}
