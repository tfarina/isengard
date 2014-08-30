#include <stdio.h>

#include <sqlite3.h>

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm


static sqlite3* db_open(const char* db_file) {
  sqlite3* db;

  if (sqlite3_open(db_file, &db)) {
    fprintf(stderr, "Failed to open database %s: %s\n", db_file,
            sqlite3_errmsg(db));
    sqlite3_close(db);
    return NULL;
  }

  return db;
}

static int db_table_create_user(sqlite3* db) {
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

  if (db_table_create_user(db)) {
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
