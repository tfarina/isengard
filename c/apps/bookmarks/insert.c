/*
 * Modified from http://zetcode.com/db/mysqlc/
 */

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

static const char kDBHost[] = "localhost";
static const char kDBUser[] = "ken";
static const char kDBPassword[] = "194304";
static const char kDBName[] = "ctestdb";

static void finish_with_error(MYSQL *sql) {
  fprintf(stderr, "%s\n", mysql_error(sql));
  mysql_close(sql);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  MYSQL *conn = NULL;
  unsigned int port = 0;

  if ((conn = mysql_init(NULL)) == NULL) {
    fprintf(stderr, "mysql_init() failed\n");
    exit(EXIT_FAILURE);
  }

  if (mysql_real_connect(conn, kDBHost, kDBUser, kDBPassword, kDBName, port,
                         NULL, 0) == NULL) {
    finish_with_error(conn);
  }

  if (mysql_query(conn,
          "INSERT INTO bookmarks VALUES (1, 'https://google.com', 'Google')")) {
    finish_with_error(conn);
  }

  mysql_close(conn);

  return 0;
}
