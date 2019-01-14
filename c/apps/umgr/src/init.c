#include <stdio.h>

#include "db.h"
#include "user.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

/* The name of the user database file.  */
static const char user_db_fname[] = "users.db";

int main(int argc, char **argv) {
  sqlite3* db;
  int rv;

  rv = db_open(user_db_fname, &db);
  if (rv != SQLITE_OK) {
    return -1;
  }

  if (user_init_database(db)) {
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
