#ifndef DRIVER_MYSQL_H_
#define DRIVER_MYSQL_H_

#include <mysql/mysql.h>

int db_mysql_connect(MYSQL **conn, const char *host, int unsigned port,
                     const char *user, const char *password, const char *dbname);

#endif /* DRIVER_MYSQL_H_ */
