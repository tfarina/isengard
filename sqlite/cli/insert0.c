#include <stdio.h>

#include <sqlite3.h>

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

int main(int argc, char* argv[]) {
  sqlite3* db;

  db = db_open("users.db");

  if (argc != 4) {
    printf("usage: %s USERNAME PASSWORD E-MAIL\n", argv[0]);
    return -1;
  }

  if (db_user_exists(db, argv[1])) {
    fprintf(stderr, "user %s already exists\n", argv[1]);
    sqlite3_close(db);
    return -1;
  }

  // Third: if user do not exist, then add the new user to the database.
  // TODO: get the arguments passed to this tool and pass to db_add_user
  // function.
  if (db_add_user(db)) {
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
