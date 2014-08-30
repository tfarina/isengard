#include <stdio.h>

#include <sqlite3.h>

static int callback(void* data, int argc, char** argv, char** column_name) {
  int i;
  fprintf(stdout, "%s: \n", (const char*)data);
  for (i = 0; i < argc; ++i) {
    printf("%-12s %s\n", column_name[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

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

static int db_table_create_user(sqlite3* db) {
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

static int db_add_user(sqlite3* db) {
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

static int db_get_user(sqlite3* db) {
  const char* sql = "SELECT login FROM user WHERE uid=1";
  const char* data = "Callback function called";

  if (sqlite3_exec(db, sql, callback, (void*)data, NULL) != SQLITE_OK) {
    fprintf(stderr, "SQLite error: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  return 0;
}

static int db_remove_user(sqlite3* db) {
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

int main(int argc, char* argv[]) {
  sqlite3* db;

  db = db_open("users.db");

  if (db_table_create_user(db)) {
    sqlite3_close(db);
    return -1;
  }

  if (db_add_user(db)) {
    sqlite3_close(db);
    return -1;
  }

  if (db_get_user(db)) {
    sqlite3_close(db);
    return -1;
  }

  if (db_remove_user(db)) {
    sqlite3_close(db);
    return -1;
  }

  if (db_get_user(db)) {
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
