#include "db_mysql.h"

#include <stdio.h>

int db_mysql_connect(MYSQL **conn, const char *host, const char *user,
                     const char *password, const char *dbname)
{
  unsigned int port = 0;
  unsigned long sql_flags = 0;

  if ((*conn = mysql_init(NULL)) == NULL) {
    fprintf(stderr, "mysql: unable to allocate memory for database connection.\n");
    return -1;
  }

  if (mysql_real_connect(*conn, host, user, password, dbname, port,
                         NULL, sql_flags) == NULL) {
    fprintf(stderr, "mysql: connection to database failed: %s\n", mysql_error(*conn));
    mysql_close(*conn);
    return -1;
  }

  return 0;
}
