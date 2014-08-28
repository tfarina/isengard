#include <stdio.h>

#include <sqlite3.h>

int main(int argc, char* argv[]) {
  sqlite3* db;
  int rc = sqlite3_open("users.db", &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s<br>\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return rc;
  }

  fprintf(stdout, "Opened 'users.db' database successfully!<br>\n");

  const char *sql = "INSERT INTO user (login, pw, email) "
                    "VALUES ('tff', 'abcdef12345678', 'tff@gmail.com');";

  rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stdout, "SQL error: %s\n<br>", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  fprintf(stdout, "User 'tff' inserted into 'user' table successfully!<br>\n");
  sqlite3_close(db);

  return 0;
}
