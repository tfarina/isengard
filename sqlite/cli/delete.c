#include <stdio.h>

#include <sqlite3.h>

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

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
  sqlite3_stmt *stmt;
  int rc;

  db = db_open("users.db");

  const char* sql = "DELETE FROM user WHERE uid=?1;";

  rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQLite error: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  sqlite3_bind_int64(stmt, 1, 1);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  sqlite3_close(db);

  return 0;
}
