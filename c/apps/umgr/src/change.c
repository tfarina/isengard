#include <stdio.h>
#include <stdlib.h>

#include "db.h"
#include "user.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

/* The name of the user database file.  */
static const char user_db_fname[] = "users.db";

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
  sqlite3* db;

  if (argc != 3) {
    fprintf(stderr, "usage: %s 'FIRST NAME' 'E-MAIL'\n", argv[0]);
    return -1;
  }

  db = db_open(user_db_fname);
  if (!db) {
    return -1;
  }

  if (user_init_database(db)) {
    db_close(db);
    return -1;
  }

  if (!user_exists(db, argv[2])) {
    fprintf(stderr, "%s: user (%s) does not exist in our database.\n", argv[0],
            argv[1]);
    db_close(db);
    return -1;
  }

  if (user_change(db, argv[1], argv[2])) {
    db_close(db);
    return -1;
  }

  db_close(db);

  return 0;
}
