#include "commands.h"

#include <stdio.h>

int cmd_change(int argc, char **argv) {
  printf("New argc count: %d\n", argc);
  printf("New argv[0]: %s\n", argv[0]);
  printf("function: cmd_change() called!\n");

  return 0;
}
