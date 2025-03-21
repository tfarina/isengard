#include <libgen.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#include "arraysize.h"
#include "commands.h"
#include "os_path.h"

static const char *g_progname;

typedef struct command_s {
        int (*run)(int argc, char **argv);
        const char *name;
        const char *alt;
        const char *description;
} command_t;

static command_t cmds[] = {
        { cmd_add,    "add",    NULL, "Creates a new contact" },
        { cmd_modify, "modify", "mod", "Modifies an existing contact" },
        { cmd_delete, "delete", "del", "Deletes the specified contact" },
        { cmd_list,   "list",   "ls", "Lists all contacts" },
};

static void usage(int status) {
        size_t i;

        fprintf(stderr, "usage: %s [--version] <command> [<args>]\n", g_progname);
        fputc('\n', stderr);
        fputs("The available commands are as follows:\n", stderr);
        fputc('\n', stderr);

        for (i = 0; i < ARRAY_SIZE(cmds); i++) {
	        command_t *cmd = cmds + i;
                fprintf(stderr, "   %-12s%s\n", cmd->name, cmd->description);
	}

	exit(status);
}

static void version(void) {
        printf("%s %s\n", g_progname, VERSION);
}

static command_t *_find_cmd(const char *name) {
        size_t i;

        for (i = 0; i < ARRAY_SIZE(cmds); i++) {
                command_t *cmd = &cmds[i];
                if (strcmp(name, cmd->name) == 0 ||
                    (cmd->alt && strcmp(name, cmd->alt) == 0)) {
                        return cmd;
                }
        }

        return NULL;
}

int main(int argc, char **argv) {
        int i;
        command_t *cmd;
        int rc;

        g_progname = os_path_basename(argv[0]);

        if (argc < 2) {
                usage(EXIT_FAILURE);
	}

	for (i = 1; i < argc; i++) {
	  if (strcmp(argv[i], "--version") == 0) {
            version();
	    exit(0);
	  }
	}

        cmd = _find_cmd(argv[1]);
        if (cmd == NULL) {
                usage(EXIT_FAILURE);
	}

        rc = cmd->run(argc - 1, argv + 1);

        return rc;
}
