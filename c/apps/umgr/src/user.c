#include "user.h"

#include <stdio.h>

#include "db.h"

int user_init_database(sqlite3* db) {
  int rv;
  sqlite3_stmt *stmt;
  const char* sql =
    "CREATE TABLE IF NOT EXISTS user ("
    "  uid INTEGER PRIMARY KEY,"     /* user id */
    "  login TEXT NOT NULL UNIQUE,"  /* username */
    "  pw TEXT NOT NULL,"            /* password */
    "  email TEXT"                   /* email */
    ");";

  if ((rv = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL)) != SQLITE_OK) {
    fprintf(stderr, "error creating user table: %s\n", sqlite3_errstr(rv));
    /* TODO: close db */
    return -1;
  }

  rv = sqlite3_step(stmt);

  sql_stmt_free(stmt);

  if (rv != SQLITE_DONE) {
    fprintf(stderr, "error creating user table: %s\n", sqlite3_errstr(rv));
    /* TODO: close db */
    return -1;
  }

  return 0;
}

int user_exists(sqlite3* db, const char* username) {
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

  sql_stmt_free(stmt);

  return rc;
}

int user_add(sqlite3* db,
             const char* username,
             const char* password,
             const char* email) {
  sqlite3_stmt* stmt;

  const char *sql = "INSERT INTO user (login, pw, email) VALUES (?1, ?2, ?3);";

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing insert statement: %s\n",
            sqlite3_errmsg(db));
    /* TODO: close db here. */
    return -1;
  }

  sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, email, -1, SQLITE_STATIC);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    fprintf(stderr, "error inserting into user table: %s\n",
            sqlite3_errmsg(db));
    return -1;
  }

  sql_stmt_free(stmt);

  return 0;
}
