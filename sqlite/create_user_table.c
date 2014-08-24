#include <stdio.h>
#include <sqlite3.h>

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

static void html_content() {
  printf("Content-type: text/html\r\n\r\n");
}

static void html_start() {
  printf("<!DOCTYPE html>");
  printf("<html>\n");
}

static void html_end() {
  printf("</body>\r\n");
  printf("</html>\r\n");
}

static const char kUserSchema[] =
  "CREATE TABLE IF NOT EXISTS 'user' ("
  "  uid INTEGER PRIMARY KEY," /* User ID */
  "  login TEXT UNIQUE,"       /* login name of the user */
  "  pw TEXT,"                 /* password */
  "  email TEXT"               /* e-mail */
  ");"
  "CREATE UNIQUE INDEX 'user_name_UNIQUE' ON 'user' ('login' ASC);";

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

  rc = sqlite3_exec(db, kUserSchema, NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n<br>\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  fprintf(stdout, "Table 'user' created successfully!<br>\n");
  sqlite3_close(db);

  html_end();
  return 0;
}
