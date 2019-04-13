#include "driver_mysql.h"

#include <stdio.h>

#include "dba.h"

typedef struct mysql_drv_data_s {
  MYSQL *mysql;
  MYSQL_RES *result;
} mysql_drv_data_t;

typedef struct mysql_drv_res_data_s {
  MYSQL_ROW row;
  MYSQL_FIELD *fields;
  long unsigned *lengths;
  long unsigned columns;
} mysql_drv_res_data_t;

static int mysql_drv_init(dba_t *handle)
{
  mysql_drv_data_t *data;

  data = calloc(1, sizeof(*data));
  if (data == NULL) {
    return -DBA_ERR_NOMEM;
  }

  handle->data = data;

  return DBA_ERR_SUCCESS;
}

static int mysql_drv_deinit(dba_t *handle)
{
  mysql_drv_data_t *data;

  data = handle->data;

  free(data->mysql);
  free(data);

  handle->data = NULL;

  return DBA_ERR_SUCCESS;
}

static int mysql_drv_connect(dba_t *handle, char const *host, int unsigned port,
                             char const *username, char const *password,
			     char const *dbname)
{
  mysql_drv_data_t *data;
  char *unix_socket_name = NULL;
  unsigned long client_flags = 0;

  data = handle->data;

  data->mysql = malloc(sizeof(MYSQL));
  if (data->mysql == NULL) {
    return -DBA_ERR_NOMEM;
  }

  mysql_init(data->mysql);

  if (mysql_real_connect(data->mysql,
                         host,
                         username,
                         password,
                         dbname,
                         port,
                         unix_socket_name,
                         client_flags) == NULL) {
    mysql_close(data->mysql);
    return -DBA_ERR_BACKEND;
  }

  return DBA_ERR_SUCCESS;
}

static int mysql_drv_disconnect(dba_t *handle)
{
  mysql_drv_data_t *data;

  data = handle->data;

  mysql_close(data->mysql);
  free(data->mysql);
  data->mysql = NULL;

  return DBA_ERR_SUCCESS;
}

static int mysql_drv_query(dba_t *handle, char const *query, long unsigned length)
{
  mysql_drv_data_t *data;

  data = handle->data;

  if (mysql_real_query(data->mysql, query, length) != 0) {
    return -DBA_ERR_BACKEND;
  }

  return DBA_ERR_SUCCESS;
}

dba_ops_t mysql_dba_ops = {
  mysql_drv_init,
  mysql_drv_deinit,
  mysql_drv_connect,
  mysql_drv_disconnect,
  mysql_drv_query
};

int db_mysql_connect(MYSQL **conn, config_t *config)
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
