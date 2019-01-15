#include <stdio.h>
#include <stdlib.h>

#include "db.h"
#include "user.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s 'E-MAIL'\n", argv[0]);
    return -1;
  }

  if (!user_exists(argv[1])) {
    fprintf(stderr, "%s: user (%s) does not exist in our database.\n",
            argv[0], argv[1]);
    return -1;
  }

  if (user_delete(argv[1])) {
    return -1;
  }

  return 0;
}
