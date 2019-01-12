#include "db_mysql.h"

#include <stdio.h>

MYSQL *db_mysql_connect(const char *host, const char *user,
                        const char *password, const char *dbname)
{
  MYSQL *conn;
  unsigned int port = 0;
  unsigned long sql_flags = 0;

  if ((conn = mysql_init(NULL)) == NULL) {
    fprintf(stderr, "mysql: unable to allocate memory for database connection.\n");
    return NULL;
  }

  if (mysql_real_connect(conn, host, user, password, dbname, port,
                         NULL, sql_flags) == NULL) {
    fprintf(stderr, "mysql: connection to database failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return NULL;
  }

  return conn;
}
