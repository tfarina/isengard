/*
 * Modified from http://zetcode.com/db/mysqlc/
 */

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#include "db.h"

static MYSQL *conn = NULL;

static int db_mysql_connect(const char *host, const char *user,
                            const char *password, const char *dbname,
                            int unsigned port)
{
  if ((conn = mysql_init(NULL)) == NULL) {
    fprintf(stderr, "mysql: unable to allocate memory for database connection.\n");
    return -1;
  }

  if (mysql_real_connect(conn, host, user, password, dbname, port,
                         NULL, 0) == NULL) {
    fprintf(stderr, "mysql: connection to database failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  return 0;
}

static int db_mysql_disconnect(void)
{
  mysql_close(conn);
  conn = NULL;
  return 0;
}

static int ab_add_contact(const char *fname, const char *lname, const char *email)
{
  char query[256];

  sprintf(query, "INSERT INTO contacts (fname, lname, email) VALUES ('%s', '%s', '%s')",
          fname, lname, email);

  if (mysql_query(conn, query)) {
    fprintf(stderr, "mysql: sql insert failed: %s\n", mysql_error(conn));
    db_mysql_disconnect();
    return -1;
  }

  return 0;
}

int main(int argc, char **argv) {
  db_config_t config;

  db_config_init(&config);

  if (db_mysql_connect(config.host, config.user, config.password, config.dbname, config.port)) {
    return -1;
  }

  if (ab_add_contact("Bill", "Gates", "gates@microsoft.com") == -1) {
    return -1;
  }

  db_mysql_disconnect();

  return 0;
}
