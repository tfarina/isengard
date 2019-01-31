#include <stdio.h>
#include <stdlib.h>

#include "user.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

int main(int argc, char **argv) {
  user_t *user = NULL;

  if (argc != 4) {
    printf("usage: %s 'FIRST NAME' 'LAST NAME' 'E-MAIL'\n", argv[0]);
    return -1;
  }

  user = user_alloc();
  user->fname = argv[1];
  user->lname = argv[2];
  user->email = argv[3];

  /* It's okay to add duplicate records. */
  if (ab_add_user(user)) {
    return -1;
  }

  return 0;
}
