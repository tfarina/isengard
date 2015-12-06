#include <libgen.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

static const char *program;

static struct builtin_cmd {
        const char *name;
        int (*exec)(int, char **);
} cmds[] = {
  { "add", cmd_add },
  { "delete", cmd_del },
  { "list", cmd_list },
};

static void usage(void) {
        static const char *usage_msg =
                "usage: %s COMMAND [ARGS]\n\n"
                "The commands are:\n"
                "   add       Adds a new user\n"
                "   delete    Deletes the specified user\n"
                "   list      Lists all users\n"
                "";
        fprintf(stderr, usage_msg, program);
        exit(EXIT_FAILURE);
}

static struct builtin_cmd *get_builtin(const char *name) {
        size_t i;
        for (i = 0; i < ARRAY_SIZE(cmds); i++) {
                struct builtin_cmd *cmd = &cmds[i];
                if (!strcmp(name, cmd->name)) {
                        printf("Command name: %s\n", cmds[i].name);
                        return cmd;
                }
        }
        return NULL;
}

int main(int argc, char **argv) {
        struct builtin_cmd *cmd;

        program = basename(argv[0]);

        if (argc < 2)
                usage();

        cmd = get_builtin(argv[1]);
        if (!cmd)
                usage();

        cmd->exec(argc - 1, argv + 1);

        return 0;
}
