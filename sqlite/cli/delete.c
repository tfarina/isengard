#include <stdio.h>
#include <stdlib.h>

#include <sqlite3.h>

#include "db.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

/* Returns 0 if the user does not exists, otherwise returns 1. */
static int db_user_exists(sqlite3* db, const char* username) {
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

static int db_user_remove(sqlite3* db, const char* username) {
  sqlite3_stmt *stmt;

  const char *sql = "DELETE FROM user WHERE login=?1;";

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "SQLite error: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  return 0;
}

int main(int argc, char* argv[]) {
  sqlite3* db;

  if (argc != 2) {
    fprintf(stderr, "usage: %s USERNAME\n", argv[0]);
    return -1;
  }

  db = db_open("users.db");
  if (!db) {
    return -1;
  }

  if (db_user_create_table(db)) {
    sqlite3_close(db);
    return -1;
  }

  if (!db_user_exists(db, argv[1])) {
    fprintf(stderr, "%s: user (%s) does not exist in our database.\n",
            argv[0], argv[1]);
    sqlite3_close(db);
    return -1;
  }

  if (db_user_remove(db, argv[1])) {
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
