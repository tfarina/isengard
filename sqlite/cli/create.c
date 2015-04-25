#include <stdio.h>

#include <sqlite3.h>

#include "db.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

static int db_user_create_table(sqlite3* db) {
  const char* sql =
    "CREATE TABLE IF NOT EXISTS 'user' ("
    "  uid INTEGER PRIMARY KEY," /* User ID */
    "  login TEXT UNIQUE,"       /* login name of the user */
    "  pw TEXT,"                 /* password */
    "  email TEXT"               /* e-mail */
    ");";

  if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
    fprintf(stderr, "SQLite error: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  return 0;
}

int main(int argc, char* argv[]) {
  sqlite3* db;

  db = db_open("users.db");
  if (!db) {
    return -1;
  }

  if (db_user_create_table(db)) {
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
