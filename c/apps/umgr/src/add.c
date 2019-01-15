#include <stdio.h>
#include <stdlib.h>

#include "db.h"
#include "user.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

int main(int argc, char **argv) {
  if (argc != 4) {
    printf("usage: %s 'FIRST NAME' 'LAST NAME' 'E-MAIL'\n", argv[0]);
    return -1;
  }

  if (user_exists(argv[3])) {
    fprintf(stderr, "%s: user (%s) already exists\n", argv[0], argv[3]);
    return -1;
  }

  if (user_add(argv[1], argv[2], argv[3])) {
    return -1;
  }

  return 0;
}
