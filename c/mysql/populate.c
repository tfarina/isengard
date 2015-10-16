/*
 * Modified from http://zetcode.com/db/mysqlc/
 */

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

void finish_with_error(MYSQL *con) {
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) {
    fprintf(stderr, "%s\n", mysql_error(con));
    exit(EXIT_FAILURE);
  }

  if (mysql_real_connect(con, "localhost", "ken", "194304", "ctestdb", 0, NULL, 0) == NULL) {
    finish_with_error(con);
  }

  if (mysql_query(con, "INSERT INTO bookmarks VALUES(1, 'https://google.com', 'Google')")) {
    finish_with_error(con);
  }

  mysql_close(con);
  return 0;
}
