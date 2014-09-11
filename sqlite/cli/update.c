#include <stdio.h>

#include <sqlite3.h>

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

static int callback(void* data, int argc, char** argv, char** column_name) {
  int i;
  //fprintf(stdout, "%s: \n", (const char*)data);
  for (i = 0; i < argc; ++i) {
    printf("%-12s %s\n", column_name[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

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

static int db_update(sqlite3* db) {
  const char *sql = "UPDATE user SET email='john@yahoo.com' WHERE uid=1; "
                    "SELECT * FROM user";
  const char* data = "Callback function called";
  if (sqlite3_exec(db, sql, callback, (void*)data, NULL) != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
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

int main(int argc, char* argv[]) {
  sqlite3* db;

  if (argc != 2) {
    fprintf(stderr, "usage: %s USERNAME\n", argv[0]);
    return -1;
  }

  db = db_open("users.db");

  if (!db_user_exists(db, argv[1])) {
    fprintf(stderr, "%s: user (%s) does not exist in our database.\n", argv[0], argv[1]);
    sqlite3_close(db);
    return -1;
  }

  // TODO:
  //       Third:  if it does not exist, blow out.
  //       Fourth: if exists, then update the email for the that user.
  if (db_update(db)) {
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
