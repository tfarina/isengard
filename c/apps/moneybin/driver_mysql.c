#include "driver_mysql.h"

#include <stdio.h>

#include "sql.h"

typedef struct mysql_drv_data_s {
  MYSQL *mysql;
  MYSQL_RES *result;
} mysql_drv_data_t;

static int db_mysql_alloc(mb_sql_connection_t *conn)
{
  mysql_drv_data_t *data;

  data = calloc(1, sizeof(*data));
  if (data == NULL) {
    return -1 /*ENOMEM*/;
  }

  conn->data = data;

  return 0;
}

static int db_mysql_free(mb_sql_connection_t *conn)
{
  mysql_drv_data_t *data;

  data = conn->data;

  free(data->mysql);
  free(data);

  conn->data = NULL;

  return 0;
}

int db_mysql_connect(MYSQL **conn, db_config_t *config)
{
  MYSQL *mysql;
  char *unix_socket_name = NULL;
  unsigned long client_flags = 0;

  mysql = malloc(sizeof(MYSQL));
  if (mysql == NULL) {
    fprintf(stderr, "mysql: unable to allocate memory for database connection.\n");
    return -1 /*ENOMEM;*/;
  }

  mysql_init(mysql);

  if (mysql_real_connect(mysql,
                         config->host,
                         config->user,
                         config->password,
                         config->dbname,
                         config->port,
                         unix_socket_name,
                         client_flags) == NULL) {
    fprintf(stderr, "mysql: connection to database failed: %s\n", mysql_error(mysql));
    mysql_close(mysql);
    return -1;
  }

  *conn = mysql;

  return 0;
}
