#include <stdio.h>

#include <sqlite3.h>

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

static sqlite3* db_open(const char* db_file) {
  sqlite3* db;

  if (sqlite3_open(db_file, &db)) {
    fprintf(stderr, "Failed to open database %s: %s\n", db_file,
            sqlite3_errmsg(db));
    sqlite3_close(db);
    return NULL;
  }

  return db;
}

static int db_init_user_table(sqlite3* db) {
  const char* sql =
    "CREATE TABLE IF NOT EXISTS 'user' ("
    "  uid INTEGER PRIMARY KEY," /* User ID */
    "  login TEXT UNIQUE,"       /* login name of the user */
    "  pw TEXT,"                 /* password */
    "  email TEXT"               /* e-mail */
    ");";

  if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
    fprintf(stderr, "SQLite error: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  return 0;
}

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

static int db_remove_user(sqlite3* db, const char* username) {
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

  if (db_init_user_table(db)) {
    sqlite3_close(db);
    return -1;
  }

  if (!db_user_exists(db, argv[1])) {
    fprintf(stderr, "%s: user (%s) does not exist in our database.\n",
            argv[0], argv[1]);
    sqlite3_close(db);
    return -1;
  }

  if (db_remove_user(db, argv[1])) {
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
