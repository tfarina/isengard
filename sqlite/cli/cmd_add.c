#include "umc.h"

#include <stdio.h>

int cmd_add(int argc, char **argv) {
  if (argc != 4) {
    printf("usage: add USERNAME PASSWORD EMAIL\n");
    return -1;
  }

  return 0;
}
