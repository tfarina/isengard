#include <stdio.h>
#include <stdlib.h>

#include "db.h"
#include "user.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

/* The name of the user database file.  */
static const char user_db_fname[] = "users.db";

int main(int argc, char **argv) {
  sqlite3 *db;
  int rv;

  if (argc != 2) {
    fprintf(stderr, "usage: %s 'E-MAIL'\n", argv[0]);
    return -1;
  }

  rv = db_open(user_db_fname, &db);
  if (rv != SQLITE_OK) {
    return -1;
  }

  if (user_init_database(db)) {
    sqlite3_close(db);
    return -1;
  }

  if (!user_exists(db, argv[1])) {
    fprintf(stderr, "%s: user (%s) does not exist in our database.\n",
            argv[0], argv[1]);
    sqlite3_close(db);
    return -1;
  }

  if (user_delete(db, argv[1])) {
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
