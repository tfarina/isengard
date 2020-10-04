#include "commands.h"

#include <stdio.h>

#include "ab.h"

int cmd_add(int argc, char **argv) {
  if (argc != 4) {
    printf("usage: add 'FIRST NAME' 'LAST NAME' 'E-MAIL'\n");
    return -1;
  }

  ab_init();


  ab_fini();

  return 0;
}
