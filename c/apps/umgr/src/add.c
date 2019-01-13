#include <stdio.h>
#include <stdlib.h>

#include "db.h"
#include "user.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

/* The name of the user database file.  */
static const char user_db_fname[] = "users.db";


int main(int argc, char **argv) {
  sqlite3* db;

  if (argc != 4) {
    printf("usage: %s 'FIRST NAME' 'LAST NAME' 'E-MAIL'\n", argv[0]);
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

  if (user_exists(db, argv[1])) {
    fprintf(stderr, "%s: user (%s) already exists\n", argv[0], argv[1]);
    db_close(db);
    return -1;
  }

  if (user_add(db, argv[1], argv[2], argv[3])) {
    db_close(db);
    return -1;
  }

  db_close(db);

  return 0;
}
