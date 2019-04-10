#include "driver_mysql.h"

#include <stdio.h>

#include "dba.h"

typedef struct mysql_drv_data_s {
  MYSQL *mysql;
  MYSQL_RES *result;
} mysql_drv_data_t;

static int mysql_drv_alloc(dba_conn_t *conn)
{
  mysql_drv_data_t *data;

  data = calloc(1, sizeof(*data));
  if (data == NULL) {
    return -1 /*ENOMEM*/;
  }

  conn->data = data;

  return 0;
}

static int mysql_drv_free(dba_conn_t *conn)
{
  mysql_drv_data_t *data;

  data = conn->data;

  free(data->mysql);
  free(data);

  conn->data = NULL;

  return 0;
}

static int mysql_drv_connect(dba_conn_t *conn)
{
  mysql_drv_data_t *data;
  char *unix_socket_name = NULL;
  unsigned long client_flags = 0;

  data = conn->data;

  data->mysql = malloc(sizeof(MYSQL));
  if (data->mysql == NULL) {
    return -1 /*ENOMEM*/;
  }

  mysql_init(data->mysql);

  if (mysql_real_connect(data->mysql,
                         conn->host,
                         conn->user,
                         conn->password,
                         conn->dbname,
                         conn->port,
                         unix_socket_name,
                         client_flags) == NULL) {
    fprintf(stderr, "mysql: connection to database failed: %s\n", mysql_error(data->mysql));
    mysql_close(data->mysql);
    return -1;
  }

  return 0;
}

static int mysql_drv_disconnect(dba_conn_t *conn)
{
  mysql_drv_data_t *data;

  data = conn->data;

  mysql_close(data->mysql);
  free(data->mysql);
  data->mysql = NULL;

  return 0;
}

static int mysql_drv_query(dba_conn_t *conn, char *query)
{
  mysql_drv_data_t *data;

  data = conn->data;

  mysql_query(data->mysql, query);

  return 0;
}

static int mysql_drv_store_result(dba_conn_t *conn)
{
  mysql_drv_data_t *data;

  data = conn->data;

  data->result = mysql_store_result(data->mysql);
  if (data->result == NULL) {
    return -1;
  }

  return 0;
}

static int mysql_drv_release_result(dba_conn_t *conn)
{
  mysql_drv_data_t *data;

  data = conn->data;

  mysql_free_result(data->result);
  data->result = NULL;

  return 0;
}

static int mysql_drv_num_columns(dba_conn_t *conn, size_t *out_ncol)
{
  mysql_drv_data_t *data;

  data = conn->data;

  *out_ncol = mysql_num_fields(data->result);

  return 0;
}

static int mysql_drv_num_rows(dba_conn_t *conn, size_t *out_nrow)
{
  mysql_drv_data_t *data;

  data = conn->data;

  *out_nrow = mysql_num_rows(data->result);

  return 0;
}

static dba_driver_ops_t mysql_drv_ops = {
  mysql_drv_alloc,
  mysql_drv_free,
  mysql_drv_connect,
  mysql_drv_disconnect
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
