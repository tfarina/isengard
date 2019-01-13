#include "db.h"

#include <stdio.h>

sqlite3* db_open(const char* db_file) {
  sqlite3* db;
  int rv;

  if ((rv = sqlite3_open(db_file, &db)) != SQLITE_OK) {
    fprintf(stderr, "error opening SQLite database %s: %s\n", db_file, sqlite3_errstr(rv));
    if ((rv = sqlite3_close(db)) != SQLITE_OK) {
      fprintf(stderr, "error closing SQLite database: %s\n", sqlite3_errstr(rv));
    }
    return NULL;
  }

  return db;
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
