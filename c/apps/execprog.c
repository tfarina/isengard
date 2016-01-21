#include <stdio.h>
#include <unistd.h>

static void
print_java_version(void)
{
        char *cmd[] = { "java", "-version", (char *)0 };
        execv("/usr/bin/java", cmd);
}

int main(void) {
        print_java_version();

        printf("unreachable\n");

        return 0;
}
