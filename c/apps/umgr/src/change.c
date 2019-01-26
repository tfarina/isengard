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
  if (argc != 3) {
    fprintf(stderr, "usage: %s 'FIRST NAME' 'E-MAIL'\n", argv[0]);
    return -1;
  }

  if (!user_exists(argv[2])) {
    fprintf(stderr, "%s: user (%s) does not exist in our database.\n", argv[0], argv[2]);
    return -1;
  }

  if (user_change(argv[1], argv[2])) {
    return -1;
  }

  return 0;
}
