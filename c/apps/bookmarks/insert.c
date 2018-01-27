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

int main(int argc, char **argv) {
  MYSQL *conn = NULL;
  unsigned int port = 0;

  if ((conn = mysql_init(NULL)) == NULL) {
    fprintf(stderr, "mysql: unable to allocate memory for database connection.\n");
    return -1;
  }

  if (mysql_real_connect(conn, kDBHost, kDBUser, kDBPassword, kDBName, port,
                         NULL, 0) == NULL) {
    fprintf(stderr, "mysql: connection to database failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  if (mysql_query(conn,
          "INSERT INTO bookmarks VALUES (1, 'https://google.com', 'Google')")) {
    fprintf(stderr, "mysql: sql insert failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  mysql_close(conn);

  return 0;
}
