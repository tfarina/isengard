// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

#include <stdio.h>
#include <stdlib.h>

#include "db.h"
#include "user.h"

/* The name of the user database file.  */
static const char user_db_fname[] = "users.db";

/* Lists records from the user table. */
static int list_user_records(sqlite3 *db) {
  sqlite3_stmt *stmt;
  const char *sql = sqlite3_mprintf("SELECT * FROM user");

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "error preparing select statement: %s\n",
            sqlite3_errmsg(db));
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

  return 0;
}

static sqlite3 *db;

static int open_user_db(void) {
  db = db_open(user_db_fname);
  if (!db) {
    return -1;
  }

  if (db_user_create_table(db)) {
    db_close(db);
    return -1;
  }

  return 0;
}

int main(int argc, char **argv) {
  int rv;

  rv = open_user_db();
  if (rv)
    return rv;

  rv = list_user_records(db);

  db_close(db);

  return rv;
}
