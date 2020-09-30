#include <libgen.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#include "arraysize.h"
#include "commands.h"

static const char *progname;

typedef struct command_s {
        const char *name;
        int (*run)(int argc, char **argv);
} command_t;

static command_t cmds[] = {
        { "add", cmd_add },
        { "change", cmd_change },
        { "delete", cmd_delete },
        { "list", cmd_list },
};

static void usage(void) {
        static const char *usage_msg =
                "usage: %s [--version] <command> [<args>]\n\n"
                "The available commands are as follows:\n\n"
                "   add       Add a new contact\n"
                "   change    Change a contact\n"
                "   delete    Delete the specified contact\n"
                "   list      List all contacts\n"
                "";
        fprintf(stderr, usage_msg, progname);
}

static void version(void) {
        printf("%s %s\n", progname, VERSION);
}

static command_t *_find_cmd(const char *name) {
        size_t i;

        for (i = 0; i < ARRAY_SIZE(cmds); i++) {
                command_t *cmd = &cmds[i];
                if (strcmp(name, cmd->name) == 0) {
                        printf("Command name: %s\n", cmds[i].name);
                        return cmd;
                }
        }

        return NULL;
}

int main(int argc, char **argv) {
        int i;
        command_t *cmd;
        int rc;

        progname = basename(argv[0]);

        if (argc < 2) {
                usage();
		return EXIT_FAILURE;
	}

	for (i = 1; i < argc; i++) {
	  if (strcmp(argv[i], "--version") == 0) {
            version();
	    exit(0);
	  }
	}

        cmd = _find_cmd(argv[1]);
        if (cmd == NULL) {
                usage();
		return EXIT_FAILURE;
	}

        rc = cmd->run(argc - 1, argv + 1);

        return rc;
}
