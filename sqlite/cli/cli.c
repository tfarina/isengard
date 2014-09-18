#include "cli.h"

#include <stdio.h>
#include <string.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

static struct {
        const char *name;
        int (*exec)(int, char **);
} cmds[] = {
  { "add", cmd_add },
};

int main(int argc, char **argv) {
        if (argc != 2)
                return 1;

        int i;
        for (i = 0; i < ARRAY_SIZE(cmds); ++i) {
                printf("%s\n", cmds[i].name);
                if (!strcmp(argv[1], cmds[i].name)) {
                        cmds[i].exec(argc, argv);
                }
        }

        return 0;
}
