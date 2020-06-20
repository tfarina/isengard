#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ffileutils.h"

int main(int argc, char **argv) {
  char buf[] = "hello, world\n";

  if (argc != 2) {
    fprintf(stderr, "usage: writefile <filename>\n");
    exit(EXIT_FAILURE);
  }

  f_write_file(argv[1], buf, sizeof(buf));

  return 0;
}
