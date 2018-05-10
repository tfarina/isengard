#include "user.h"

#include <stdio.h>

#include "db.h"

/* The name of the user database file.  */
static const char user_db_fname[] = "users.db";

static sqlite3 *user_db;
static sqlite3_stmt *user_insert_stmt;

int user_open_db(void) {
  if (user_db) {
    return 0; /* Already open. */
  }

  user_db = db_open(user_db_fname);
  if (!user_db) {
    fprintf(stderr, "error opening user database\n");
    return -1;
  }

  if (user_init_database(user_db)) {
    db_close(user_db);
    return -1;
  }

  return 0;
}

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
    db_close(db);
    return -1;
  }

  rv = sqlite3_step(stmt);

  sql_stmt_free(stmt);

  if (rv != SQLITE_DONE) {
    fprintf(stderr, "error creating user table: %s\n", sqlite3_errstr(rv));
    db_close(db);
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
    db_close(db);
    return -1;
  }

  user_insert_stmt = stmt;

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

int user_delete(sqlite3 *db, const char *username) {
  sqlite3_stmt *stmt;

  const char *sql = "DELETE FROM user WHERE login=?1;";

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing delete statement: %s\n",
            sqlite3_errmsg(db));
    return -1;
  }

  sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    fprintf(stderr, "error deleting user from table: %s\n",
            sqlite3_errmsg(db));
    return -1;
  }

  sql_stmt_free(stmt);

  return 0;
}

/* Lists records from the user table. */
int user_list(void) {
  int err;

  err = user_open_db();
  if (err)
    return err;

  sqlite3_stmt *stmt;
  const char *sql = sqlite3_mprintf("SELECT * FROM user");

  if (sqlite3_prepare_v2(user_db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing select statement: %s\n",
            sqlite3_errmsg(user_db));
    return -1;
  }

  // http://www.csl.mtu.edu/cs1141/www/examples/sqlite/sqlite_select.c
  int col_width[] = {12, 9, 5, 19, 0};

  printf("      uid   |    login  |     pw    |         email     \n");
  printf(" -----------+-----------+-----------+-------------------\n");

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    for (int i = 0; i < sqlite3_column_count(stmt); i++) {
      printf(" %*s", col_width[1], sqlite3_column_text(stmt, i));
      if (i < sqlite3_column_count(stmt) - 1) {
        printf("  | ");
      }
    }
    printf("\n");
  }

  sql_stmt_free(stmt);

  db_close(user_db);

  return err;
}
