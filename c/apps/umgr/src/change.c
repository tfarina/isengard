#include <stdio.h>
#include <stdlib.h>

#include "user.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

// static int callback(void* data, int argc, char** argv, char** column_name) {
//   int i;
//   //fprintf(stdout, "%s: \n", (const char*)data);
//   for (i = 0; i < argc; ++i) {
//     printf("%-12s %s\n", column_name[i], argv[i] ? argv[i] : "NULL");
//   }
//   printf("\n");
//   return 0;
// }

int main(int argc, char **argv) {
  alpm_list_t *users;
  int uid;
  user_t *user = NULL;

  if (argc != 5) {
    fprintf(stderr, "usage: %s 'USER-ID' 'FIRST NAME' 'LAST NAME' 'E-MAIL'\n", argv[0]);
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

  user->fname = argv[2];
  user->lname = argv[3];
  user->email = argv[4];

  if (ab_change_user(user)) {
    return -1;
  }

  return 0;
}
