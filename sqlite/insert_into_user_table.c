#include <stdio.h>
#include <sqlite3.h>

static void html_content() {
  printf("Content-type: text/html\r\n\r\n");
}

static void html_start() {
  printf("<!DOCTYPE html>\n");
  printf("<html>\n");
}

static void html_end() {
  printf("</body>\r\n");
  printf("</html>\r\n");
}

int main(int argc, char* argv[]) {
  html_content();
  html_start();
  printf("<head>\n");
  printf("</head>\n");
  printf("<body>\n");

  sqlite3* db;
  int rc = sqlite3_open("users.db", &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s<br>\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return rc;
  }

  fprintf(stdout, "Opened 'users.db' database successfully!<br>\n");

  const char* sql =
      "INSERT INTO user (login, pw, email) "
      "VALUES ('tff', 'abcdef12345678', 'tff@gmail.com');";
  rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stdout, "SQL error: %s\n<br>", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  fprintf(stdout,
          "User 'tff' inserted into 'user' table successfully!<br>\n");
  sqlite3_close(db);

  html_end();
  return 0;
}
