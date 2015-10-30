#include "db.h"

#include <stdio.h>

sqlite3* db_open(const char* db_file) {
  sqlite3* db;

  if (sqlite3_open(db_file, &db) != SQLITE_OK) {
    fprintf(stderr, "error opening %s: %s\n", db_file, sqlite3_errmsg(db));
    if (sqlite3_close(db) != SQLITE_OK) {
      fprintf(stderr, "failed to close the user db: %s\n", sqlite3_errmsg(db));
    }
    return NULL;
  }

  return db;
}

