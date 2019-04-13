#include <stdlib.h>

#include <mysql/mysql.h>

#include "dba.h"

typedef struct mysql_dba_data_s {
  MYSQL *mysql;
  int first_result; /* whether result_init was already called or not */
} mysql_dba_data_t;

typedef struct mysql_dba_res_data_s {
  MYSQL_RES *result;
  MYSQL_ROW row;
  MYSQL_FIELD *fields;
  long unsigned *lengths;
  long unsigned columns;
} mysql_dba_res_data_t;

static int mysql_dba_init(dba_t *handle)
{
  mysql_dba_data_t *data;

  data = calloc(1, sizeof(*data));
  if (data == NULL) {
    return -DBA_ERR_NOMEM;
  }

  handle->data = data;

  return DBA_ERR_SUCCESS;
}

static int mysql_dba_deinit(dba_t *handle)
{
  mysql_dba_data_t *data;

  data = handle->data;

  free(data->mysql);
  free(data);

  handle->data = NULL;

  return DBA_ERR_SUCCESS;
}

static int mysql_dba_connect(dba_t *handle, char const *host, int unsigned port,
                             char const *username, char const *password,
			     char const *dbname)
{
  mysql_dba_data_t *data;
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

static int mysql_dba_disconnect(dba_t *handle)
{
  mysql_dba_data_t *data;

  data = handle->data;

  mysql_close(data->mysql);
  free(data->mysql);
  data->mysql = NULL;

  return DBA_ERR_SUCCESS;
}

static int mysql_dba_query(dba_t *handle, char const *query, long unsigned length)
{
  mysql_dba_data_t *data;

  data = handle->data;

  if (mysql_real_query(data->mysql, query, length) != 0) {
    return -DBA_ERR_BACKEND;
  }

  data->first_result = 1;

  return DBA_ERR_SUCCESS;
}

static int mysql_dba_result_init(dba_t *handle, dba_result_t **result)
{
  dba_result_t *res;
  mysql_dba_data_t *data;
  mysql_dba_res_data_t *res_data;

  data = handle->data;
  if (data->first_result == 0) {
    return DBA_RES_DONE;
  }

  data->first_result = 0;

  res = malloc(sizeof(dba_result_t));
  if (res == NULL) {
    return -DBA_ERR_NOMEM;
  }

  *result = res;

  res_data = malloc(sizeof(mysql_dba_res_data_t));
  if (data == NULL) {
    free(res);
    res = NULL;

    return -DBA_ERR_NOMEM;
  }

  res->data = res_data;

  res_data->columns = (long unsigned)mysql_field_count(data->mysql);
  if (res_data->columns == 0) {
    return DBA_RES_NOROWS;  /* It means empty or it was not a SELECT statement. */
  }

  res_data->result = mysql_store_result(data->mysql);
  if (res_data->result == NULL) {
    free(res_data);
    *result = NULL;

    return -DBA_ERR_BACKEND;
  }

  res_data->fields = mysql_fetch_fields(res_data->result);

  return DBA_RES_ROWS;
}

static int mysql_dba_result_deinit(dba_result_t *result)
{
  mysql_dba_res_data_t *res_data;

  res_data = result->data;

  if (res_data == NULL) {
    return -DBA_ERR_PARAM;
  }

  if (res_data->result != NULL) {
    mysql_free_result(res_data->result);
    res_data->result = NULL;
  }

  free(res_data);
  res_data = NULL;

  free(result);

  return DBA_ERR_SUCCESS;
}

static int mysql_dba_result_fetch_row(dba_result_t *result)
{
  mysql_dba_res_data_t *res_data;
  MYSQL_RES *res;

  res_data = result->data;

  if (res_data == NULL) {
    return -DBA_ERR_PARAM;
  }

  res = res_data->result;
  if (res == NULL) {
    return -DBA_ERR_PARAM;
  }

  res_data->row = mysql_fetch_row(res);
  if (res_data->row == NULL) {
    res_data->lengths = NULL;

    return DBA_ROW_DONE;
  }

  res_data->lengths = mysql_fetch_lengths(res);

  return DBA_ROW_NEXT;
}

dba_ops_t mysql_dba_ops = {
  mysql_dba_init,
  mysql_dba_deinit,
  mysql_dba_connect,
  mysql_dba_disconnect,
  mysql_dba_query,
  mysql_dba_result_init,
  mysql_dba_result_deinit
};
