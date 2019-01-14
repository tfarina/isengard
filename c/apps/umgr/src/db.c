#include "db.h"

#include <stdio.h>

int db_open(const char *filename, sqlite3 **db) {
  int rv;

  *db = NULL;

  rv = sqlite3_open(filename, db);
  if (rv != SQLITE_OK) {
    fprintf(stderr, "error opening SQLite database %s: %s\n", filename, sqlite3_errstr(rv));
    if ((rv = sqlite3_close(*db)) != SQLITE_OK) {
      fprintf(stderr, "error closing SQLite database: %s\n", sqlite3_errstr(rv));
    }
    return rv;
  }

  return SQLITE_OK;
}

void db_close(sqlite3 *db) {
  int rv;

  if ((rv = sqlite3_close(db)) != SQLITE_OK) {
    fprintf(stderr, "error closing SQLite database: %s\n", sqlite3_errstr(rv));
  }
}

void sql_stmt_free(sqlite3_stmt *stmt) {
  sqlite3_finalize(stmt);
  stmt = NULL;
}
