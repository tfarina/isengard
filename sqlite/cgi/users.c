#include <stdio.h>

#include <sqlite3.h>

#include "html.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

int main(int argc, char* argv[]) {
  html_content();
  html_start();
  printf("<head>\n");
  printf("</head>\n");
  printf("<body>\n");

  sqlite3* db;
  int rc = sqlite3_open("users.db", &db);
  if (rc) {
    fprintf(stdout, "Can't open database: %s<br>\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return rc;
  }

  //fprintf(stdout, "Opened 'users.db' database successfully!<br>\n");

  sqlite3_stmt* stmt;
  const char* sql = sqlite3_mprintf("SELECT * FROM user");

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "SQLite error: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  // http://www.csl.mtu.edu/cs1141/www/examples/sqlite/sqlite_select.c

  printf("<table border=\"1px\" style=\"border-collapse: collapse\">\n");
  printf("<tr><th>uid</th><th>login</th><th>pw</th><th>email</th></tr>\n");

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    printf("<tr>\n");
    for (int i = 0; i < sqlite3_column_count(stmt); i++) {
      printf("<td>%s</td>\n", sqlite3_column_text(stmt, i));
    }
    printf("</tr>\n");
  }
  printf("</table\n");

  sqlite3_finalize(stmt);

  fprintf(stdout, "Selected everything from 'users' table successfully!<br>\n");
  sqlite3_close(db);

  html_end();
  return 0;
}
