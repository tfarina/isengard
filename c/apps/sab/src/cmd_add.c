#include "commands.h"

#include <stdio.h>

int cmd_add(int argc, char **argv) {
  if (argc != 4) {
    printf("usage: add 'FIRST NAME' 'LAST NAME' 'E-MAIL'\n");
    return -1;
  }

  return 0;
}
