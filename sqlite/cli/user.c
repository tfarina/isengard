#include "user.h"

#include <stdio.h>

int db_user_create_table(sqlite3* db) {
  int rv;
  sqlite3_stmt *stmt;
  const char* sql =
    "CREATE TABLE IF NOT EXISTS 'user' ("
    "  uid INTEGER PRIMARY KEY," /* User ID */
    "  login TEXT NOT NULL UNIQUE," /* login name of the user */
    "  pw TEXT NOT NULL,"        /* password */
    "  email TEXT"               /* e-mail */
    ");";

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error creating user table: %s\n", sqlite3_errmsg(db));
    /* TODO: close db */
    return -1;
  }

  rv = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  if (rv != SQLITE_DONE) {
    fprintf(stderr, "error creating user table: %s\n", sqlite3_errstr(rv));
    /* TODO: close db */
    return -1;
  }

  return 0;
}

int db_user_exists(sqlite3* db, const char* username) {
  sqlite3_stmt* stmt;
  int rc;

  char* sql = sqlite3_mprintf("SELECT 1 FROM user WHERE login=%Q", username);

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "SQLite error: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_step(stmt) != SQLITE_ROW) {
    rc = 0;
  } else {
    rc = 1;
  }

  sqlite3_finalize(stmt);
  return rc;
}
