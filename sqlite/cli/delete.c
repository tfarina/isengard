#include <stdio.h>

#include <sqlite3.h>

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

int main(int argc, char* argv[]) {
  sqlite3* db;
  sqlite3_stmt *stmt;

  int rc = sqlite3_open("users.db", &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return rc;
  }

  const char* sql = "DELETE FROM user WHERE uid=?1;";

  rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "%s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  sqlite3_bind_int64(stmt, 1, 1);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  sqlite3_close(db);

  return 0;
}
