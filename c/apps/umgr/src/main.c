#include <libgen.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

static const char *progname;

static struct builtin_cmd {
        const char *name;
        int (*exec)(int, char **);
} cmds[] = {
  { "list", cmd_list },
  { "add", cmd_add },
  { "update", cmd_update },
  { "delete", cmd_del },
};

static void usage(void) {
        static const char *usage_msg =
                "usage: %s COMMAND [ARGS]\n\n"
                "The commands are:\n"
                "   list      Lists all users\n"
                "   add       Adds a new user\n"
                "   update    Update a user\n"
                "   delete    Deletes the specified user\n"
                "";
        fprintf(stderr, usage_msg, progname);
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

        progname = basename(argv[0]);

        if (argc < 2)
                usage();

        cmd = get_builtin(argv[1]);
        if (!cmd)
                usage();

        cmd->exec(argc - 1, argv + 1);

        return 0;
}
