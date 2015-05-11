#include "umc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

static struct {
        const char *name;
        int (*exec)(int, char **);
} cmds[] = {
  { "add", cmd_add },
  { "del", cmd_del },
  { "list", cmd_list },
};

static void usage(void) {
#define FMT                              \
"\n usage: umc COMMAND [ARGS]\n"         \
"\n The commands are:\n"                 \
"   add       Add a new user\n"          \
"   del       Delete an existing user\n" \
"   list      List all users\n"          \
"\n"

        fprintf(stderr, FMT);
#undef FMT
        exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
        if (argc < 2)
                usage();

        int i;
        for (i = 0; i < ARRAY_SIZE(cmds); ++i) {
                if (!strcmp(argv[1], cmds[i].name)) {
                        printf("Command name: %s\n", cmds[i].name);
                        cmds[i].exec(argc - 1, argv + 1);
                }
        }

        return 0;
}
