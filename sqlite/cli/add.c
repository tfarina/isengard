#include "cli.h"

#include <stdio.h>

int cmd_add(int argc, char **argv) {
        printf("New argc count: %d\n", argc);
        printf("cmd name: %s\n", argv[0]);
        printf("function: cmd_add() called!\n");

        return 0;
}
