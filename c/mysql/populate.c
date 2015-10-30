/*
 * Modified from http://zetcode.com/db/mysqlc/
 */

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

static void finish_with_error(MYSQL *sql) {
  fprintf(stderr, "%s\n", mysql_error(sql));
  mysql_close(sql);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  MYSQL *sql = NULL;

  if ((sql = mysql_init(NULL)) == NULL) {
    fprintf(stderr, "mysql_init() failed\n");
    exit(EXIT_FAILURE);
  }

  if (mysql_real_connect(sql, "localhost", "ken", "194304", "ctestdb", 0, NULL, 0) == NULL) {
    finish_with_error(sql);
  }

  if (mysql_query(sql, "INSERT INTO bookmarks VALUES(1, 'https://google.com', 'Google')")) {
    finish_with_error(sql);
  }

  mysql_close(sql);

  return 0;
}
