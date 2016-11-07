#include <stdio.h>

#include "db.h"
#include "user.h"

/* The name of the user database file.  */
static const char user_db_fname[] = "users.db";

static int callback(void* data, int argc, char** argv, char** column_name) {
  int i;
  fprintf(stdout, "%s: \n", (const char*)data);
  for (i = 0; i < argc; ++i) {
    printf("%-12s %s\n", column_name[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

static int db_user_add(sqlite3* db) {
  sqlite3_stmt* stmt;

  const char *sql = "INSERT INTO user (login, pw, email) VALUES (?1, ?2, ?3);";

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "SQLite error: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  sqlite3_bind_text(stmt, 1, "jrp", -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, "abcdef123", -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, "jrp@hotmail.com", -1, SQLITE_STATIC);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  return 0;
}

static int db_user_get(sqlite3* db) {
  const char* sql = "SELECT login FROM user WHERE uid=1";
  const char* data = "Callback function called";

  if (sqlite3_exec(db, sql, callback, (void*)data, NULL) != SQLITE_OK) {
    fprintf(stderr, "SQLite error: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  return 0;
}

static int db_user_remove(sqlite3* db) {
  sqlite3_stmt *stmt;

  const char *sql = "DELETE FROM user WHERE uid=?1;";

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "SQLite error: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  sqlite3_bind_int64(stmt, 1, 1);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  return 0;
}

int main(int argc, char **argv) {
  sqlite3* db;

  db = db_open(user_db_fname);

  if (db_user_create_table(db)) {
    sqlite3_close(db);
    return -1;
  }

  if (db_user_add(db)) {
    sqlite3_close(db);
    return -1;
  }

  if (db_user_get(db)) {
    sqlite3_close(db);
    return -1;
  }

  if (db_user_remove(db)) {
    sqlite3_close(db);
    return -1;
  }

  if (db_user_get(db)) {
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
