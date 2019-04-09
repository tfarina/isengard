#ifndef DRIVER_MYSQL_H_
#define DRIVER_MYSQL_H_

#include <mysql/mysql.h>

#include "config.h"

int db_mysql_connect(MYSQL **conn, config_t *config);

#endif /* DRIVER_MYSQL_H_ */
