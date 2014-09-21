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

static int db_add_user(sqlite3* db,
                       const char* username,
                       const char* password,
                       const char* email) {
  sqlite3_stmt* stmt;

  const char *sql = "INSERT INTO user (login, pw, email) VALUES (?1, ?2, ?3);";

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "SQLite error: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, email, -1, SQLITE_STATIC);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  return 0;
}

int main(int argc, char* argv[]) {
  sqlite3* db;

  if (argc != 4) {
    printf("usage: %s USERNAME PASSWORD E-MAIL\n", argv[0]);
    return -1;
  }

  db = db_open("users.db");

  // TODO(tfarina): If the table does not exists, then we have a problem here.
  // How come we will insert an entry in a table that does not exist?
  // We should check if it exist, if not then create.
  // It is pretty easy to reproduce this:
  // Just remove users.db then run out/insert0 ....

  if (db_user_exists(db, argv[1])) {
    // FIXME: We show this message even if the TABLE does not exist. :(
    // This shouldn't happen!
    fprintf(stderr, "%s: user (%s) already exists\n", argv[0], argv[1]);
    sqlite3_close(db);
    return -1;
  }

  if (db_add_user(db, argv[1], argv[2], argv[3])) {
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
