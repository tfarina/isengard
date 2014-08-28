#include <stdio.h>

#include <sqlite3.h>

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

static const char kDbName[] = "users.db";

int main(int argc, char* argv[]) {
  sqlite3* db;
  int rc = sqlite3_open(kDbName, &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return rc;
  }
  sqlite3_close(db);

  return 0;
}
