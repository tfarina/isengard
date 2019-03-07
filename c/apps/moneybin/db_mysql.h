#ifndef DB_MYSQL_H_
#define DB_MYSQL_H_

#include <mysql/mysql.h>

MYSQL *db_mysql_connect(const char *host, const char *user,
                        const char *password, const char *dbname);

#endif /* DB_MYSQL_H_ */
