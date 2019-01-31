#include "user.h"

#include <stdio.h>

#include "third_party/sqlite/amalgamation/sqlite3.h"

#include "db.h"

/* The name of the user database file.  */
static const char user_db_fname[] = "users.db";

/* This is the column separator. */
#define SEP_COL "|"

/**
 * Makes sure the 'user' table is created if it does not exist yet.
 *
 * @return return 0 on success, -1 otherwise.
 */
static int _create_tables(sqlite3* db) {
  int rc;
  sqlite3_stmt *stmt;
  const char sql[] =
    "CREATE TABLE IF NOT EXISTS user ("
    "  uid INTEGER PRIMARY KEY,"     /* user id */
    "  fname TEXT,"                  /* first name */
    "  lname TEXT,"                  /* last name */
    "  email TEXT"                   /* email */
    ");";

  if ((rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL)) != SQLITE_OK) {
    fprintf(stderr, "error preparing create statement: %s\n", sqlite3_errstr(rc));
    db_close(db);
    return -1;
  }

  rc = sqlite3_step(stmt);

  sqlite3_finalize(stmt);
  stmt = NULL;

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "error creating user table: %s\n", sqlite3_errstr(rc));
    db_close(db);
    return -1;
  }

  return 0;
}

user_t *user_alloc(void) {
  user_t *user = NULL;

  user = malloc(sizeof(user_t));
  if (user == NULL) {
    return NULL;
  }

  user->fname = NULL;
  user->lname = NULL;
  user->email = NULL;

  return user;
}

static sqlite3* conn = NULL;

int ab_init(void) {
  int rc;

  /* Do nothing if the database handle has been set. */
  if (conn) {
    return 0;
  }

  rc = db_open(user_db_fname, &conn);
  if (rc != SQLITE_OK) {
    return -1;
  }

  if (_create_tables(conn)) {
    db_close(conn);
    return -1;
  }

  return 0;
}

int ab_close(void) {
  db_close(conn);
  conn = NULL;
}

int ab_add_user(user_t *user) {
  const char *sql;
  sqlite3_stmt *stmt;

  sql = "INSERT INTO user (fname, lname, email) VALUES (?1, ?2, ?3);";

  if (sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing insert statement: %s\n",
            sqlite3_errmsg(conn));
    db_close(conn);
    return -1;
  }

  sqlite3_bind_text(stmt, 1, user->fname, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, user->lname, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, user->email, -1, SQLITE_STATIC);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    fprintf(stderr, "error inserting into user table: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_finalize(stmt);
  stmt = NULL;

  return 0;
}

int ab_change_user(user_t *user) {
  const char *sql;
  sqlite3_stmt *stmt;
  int rc;

  sql = "UPDATE user SET fname=?2, lname=?3, email=?4 WHERE uid=?1;";

  if (sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing update statement: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_bind_int(stmt, 1, user->id);

  rc = sqlite3_bind_text(stmt, 2, user->fname, -1, SQLITE_STATIC);

  if (rc == SQLITE_OK)
    rc = sqlite3_bind_text(stmt, 3, user->lname, -1, SQLITE_STATIC);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "error binding a value for the user table: %s\n",
            sqlite3_errmsg(conn));
  }

  rc = sqlite3_bind_text(stmt, 4, user->email, -1, SQLITE_STATIC);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    fprintf(stderr, "error updating user table: %s\n", sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_finalize(stmt);
  stmt = NULL;

  return 0;
}

int ab_delete_user(user_t *user) {
  const char *sql;
  sqlite3_stmt *stmt;

  sql = "DELETE FROM user WHERE uid=?1;";

  if (sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing delete statement: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_bind_int(stmt, 1, user->id);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    fprintf(stderr, "error deleting user from table: %s\n",
            sqlite3_errmsg(conn));
    return -1;
  }

  sqlite3_finalize(stmt);
  stmt = NULL;

  return 0;
}

alpm_list_t *ab_get_user_list(void) {
  const char *sql;
  sqlite3_stmt *stmt;
  alpm_list_t *users = NULL;

  sql = "SELECT * FROM user";

  if (sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing select statement: %s\n",
            sqlite3_errmsg(conn));
    return NULL;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    user_t *user = user_alloc();
    user->id = sqlite3_column_int(stmt, 0);
    user->fname = strdup((const char *)sqlite3_column_text(stmt, 1));
    user->lname = strdup((const char *)sqlite3_column_text(stmt, 2));
    user->email = strdup((const char *)sqlite3_column_text(stmt, 3));
    users = alpm_list_add(users, user);
  }

  sqlite3_finalize(stmt);
  stmt = NULL;

  return users;
}

user_t *ab_get_user_by_id(alpm_list_t *users, int id) {
  alpm_list_t *i;

  for (i = users; i; i = alpm_list_next(i)) {
    user_t *user = (user_t *)i->data;
    if (user->id == id) {
      return user;
    }
  }

  return NULL;
}

int ab_print_user_records(void) {
  alpm_list_t *users, *i;

  users = ab_get_user_list();

  for (i = users; i; i = alpm_list_next(i)) {
    user_t *user = i->data;
    printf("%d|%s|%s|%s\n", user->id, user->fname, user->lname, user->email);
  }

  return 0;
}
