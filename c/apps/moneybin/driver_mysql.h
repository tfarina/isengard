#ifndef DRIVER_MYSQL_H_
#define DRIVER_MYSQL_H_

#include <mysql/mysql.h>

typedef struct mb_sql_connection_s {
  void *data;
} mb_sql_connection_t;

int db_mysql_alloc(mb_sql_connection_t *conn);
int db_mysql_free(mb_sql_connection_t *conn);

int db_mysql_connect(MYSQL **conn, const char *host, int unsigned port,
                     const char *user, const char *password, const char *dbname);

#endif /* DRIVER_MYSQL_H_ */
