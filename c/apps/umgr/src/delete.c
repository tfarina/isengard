#include <stdio.h>
#include <stdlib.h>

#include "db.h"
#include "user.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

/* The name of the user database file.  */
static const char user_db_fname[] = "users.db";

static int user_delete(sqlite3 *db, const char *username) {
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

int main(int argc, char **argv) {
  sqlite3 *db;

  if (argc != 2) {
    fprintf(stderr, "usage: %s USERNAME\n", argv[0]);
    return -1;
  }

  db = db_open(user_db_fname);
  if (!db) {
    return -1;
  }

  if (db_user_create_table(db)) {
    sqlite3_close(db);
    return -1;
  }

  if (!user_exists(db, argv[1])) {
    fprintf(stderr, "%s: user (%s) does not exist in our database.\n",
            argv[0], argv[1]);
    sqlite3_close(db);
    return -1;
  }

  if (user_delete(db, argv[1])) {
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
