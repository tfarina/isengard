#include <stdio.h>

#include <sqlite3.h>

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

static int db_update(sqlite3* db, const char *username, const char *email) {
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

int main(int argc, char* argv[]) {
  sqlite3* db;

  if (argc != 3) {
    fprintf(stderr, "usage: %s USERNAME E-MAIL\n", argv[0]);
    return -1;
  }

  db = db_open("users.db");

  if (!db_user_exists(db, argv[1])) {
    fprintf(stderr, "%s: user (%s) does not exist in our database.\n", argv[0],
            argv[1]);
    sqlite3_close(db);
    return -1;
  }

  if (db_update(db, argv[1], argv[2])) {
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
