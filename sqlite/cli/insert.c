#include <stdio.h>

#include <sqlite3.h>

int main(int argc, char* argv[]) {
  sqlite3* db;
  sqlite3_stmt* stmt;

  int rc = sqlite3_open("users.db", &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return rc;
  }

  const char *sql = "INSERT INTO user (login, pw, email) VALUES (?1, ?2, ?3);";

  rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "%s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  sqlite3_bind_text(stmt, 1, "jrp", -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, "abcdef123", -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, "jrp@hotmail.com", -1, SQLITE_STATIC);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  sqlite3_close(db);

  return 0;
}
