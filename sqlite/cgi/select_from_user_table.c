#include <stdio.h>

#include <sqlite3.h>

#include "html.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

static int callback(void* data, int argc, char** argv, char** column_name) {
  int i;
  fprintf(stdout, "<br>%s: <br><br>\n", (const char*)data);
  for (i = 0; i < argc; ++i) {
    printf("%s = %s<br>\n", column_name[i], argv[i] ? argv[i] : "NULL");
  }
  printf("<br>\n");
  return 0;
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
    fprintf(stdout, "Can't open database: %s<br>\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return rc;
  }

  fprintf(stdout, "Opened 'users.db' database successfully!<br>\n");

  const char* sql = "SELECT * FROM user";
  const char* data = "Callback function called";
  rc = sqlite3_exec(db, sql, callback, (void*)data, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stdout, "SQL error: %s\n<br>", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  fprintf(stdout, "Selected everything from 'users' table successfully!<br>\n");
  sqlite3_close(db);

  html_end();
  return 0;
}
