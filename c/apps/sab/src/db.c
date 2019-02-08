#include "db.h"

#include <stdio.h>

int db_open(const char *filename, sqlite3 **db) {
  int rc;

  *db = NULL;

  rc = sqlite3_open(filename, db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "error opening SQLite database %s: %s\n", filename, sqlite3_errmsg(*db));
    if ((rc = sqlite3_close(*db)) != SQLITE_OK) {
      fprintf(stderr, "error closing SQLite database: %s\n", sqlite3_errmsg(*db));
    }
    return -1;
  }

  return 0;
}

void db_close(sqlite3 *db) {
  int rc;

  if ((rc = sqlite3_close(db)) != SQLITE_OK) {
    fprintf(stderr, "error closing SQLite database: %s\n", sqlite3_errmsg(db));
  }
}
