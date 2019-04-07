#ifndef DRIVER_MYSQL_H_
#define DRIVER_MYSQL_H_

#include <mysql/mysql.h>

#include "db.h"

int db_mysql_connect(MYSQL **conn, db_config_t *config);

#endif /* DRIVER_MYSQL_H_ */
