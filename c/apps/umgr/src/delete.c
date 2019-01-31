#include <stdio.h>
#include <stdlib.h>

#include "user.h"

int main(int argc, char **argv) {
  alpm_list_t *users;
  int uid;
  user_t *user = NULL;

  if (argc != 2) {
    fprintf(stderr, "usage: %s 'USER-ID'\n", argv[0]);
    return -1;
  }

  users = ab_get_user_list();

  uid = atoi(argv[1]);

  user = ab_get_user_by_id(users, uid);
  if (!user) {
    fprintf(stderr, "%s: user (%s) does not exist in our database.\n",
            argv[0], argv[1]);
    return -1;
  }

  if (ab_delete_user(user)) {
    return -1;
  }

  return 0;
}
