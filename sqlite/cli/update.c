#include <stdio.h>
#include <stdlib.h>

#include <sqlite3.h>

#include "db.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

// static int callback(void* data, int argc, char** argv, char** column_name) {
//   int i;
//   //fprintf(stdout, "%s: \n", (const char*)data);
//   for (i = 0; i < argc; ++i) {
//     printf("%-12s %s\n", column_name[i], argv[i] ? argv[i] : "NULL");
//   }
//   printf("\n");
//   return 0;
// }

static int db_user_create_table(sqlite3* db) {
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

static int db_update_user_email(sqlite3* db,
                                const char *username, const char *email) {
  sqlite3_stmt *stmt;

  const char *sql = "UPDATE user SET email=?1 WHERE login=?2;";

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "SQLite error: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  return 0;
}

int main(int argc, char* argv[]) {
  sqlite3* db;

  if (argc != 3) {
    fprintf(stderr, "usage: %s USERNAME E-MAIL\n", argv[0]);
    return -1;
  }

  db = db_open("users.db");
  if (!db) {
    return EXIT_FAILURE;
  }

  if (db_user_create_table(db)) {
    sqlite3_close(db);
    return -1;
  }

  if (!db_user_exists(db, argv[1])) {
    fprintf(stderr, "%s: user (%s) does not exist in our database.\n", argv[0],
            argv[1]);
    sqlite3_close(db);
    return -1;
  }

  if (db_update_user_email(db, argv[1], argv[2])) {
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
