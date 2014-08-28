#include <stdio.h>

#include <sqlite3.h>

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

static const char kUserSchema[] =
  "CREATE TABLE IF NOT EXISTS 'user' ("
  "  uid INTEGER PRIMARY KEY," /* User ID */
  "  login TEXT UNIQUE,"       /* login name of the user */
  "  pw TEXT,"                 /* password */
  "  email TEXT"               /* e-mail */
  ");";

static sqlite3* db_open(const char* db_file) {
  sqlite3* db;

  if (sqlite3_open(db_file, &db)) {
    fprintf(stderr, "Failed to open database %s: %s\n", "users.db",
            sqlite3_errmsg(db));
    sqlite3_close(db);
    return NULL;
  }

  return db;
}

int main(int argc, char* argv[]) {
  sqlite3* db;
  int rc;

  sqlite3_initialize();

  db = db_open("users.db");

  rc = sqlite3_exec(db, kUserSchema, NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  sqlite3_close(db);
  sqlite3_shutdown();

  return 0;
}
