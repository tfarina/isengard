#ifndef DB_MYSQL_H_
#define DB_MYSQL_H_

#include <mysql/mysql.h>

int db_mysql_connect(MYSQL **conn, const char *host, const char *user,
                     const char *password, const char *dbname);

#endif /* DB_MYSQL_H_ */
