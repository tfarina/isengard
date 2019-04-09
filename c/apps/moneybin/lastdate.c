#include <stdio.h>
#include <string.h>

#include "config.h"
#include "driver_mysql.h"

static int get_column_data(MYSQL_RES *result, size_t nrow, size_t ncol, char **outdata)
{
  size_t num_rows;
  MYSQL_ROW row;

  num_rows = mysql_num_rows(result);
  if (num_rows <= 0) {
    fprintf(stderr, "No rows found\n");
    return -1;
  }

  mysql_data_seek(result, nrow);
  row = mysql_fetch_row(result);
  if (row == NULL) {
    fprintf(stderr, "No row found\n");
    return -1;
  }

  *outdata = row[ncol];

  return 0;
}

int main(void)
{
  config_t config;
  MYSQL *conn;
  int rc;
  char query[256];
  MYSQL_RES *res = NULL;
  char *lastdate;

  config_init(&config);

  rc = db_mysql_connect(&conn, &config);
  if (rc < 0) {
    return -1;
  }

  sprintf(query, "SELECT max(date) FROM historicaldata");

  if (mysql_query(conn, query)) {
    fprintf(stderr, "mysql: sql select failed: %s\n", mysql_error(conn));
    //db_mysql_disconnect();
    return -1;
  }

  res = mysql_store_result(conn);
  if (res == NULL) {
    fprintf(stderr, "mysql: sql result retrieval failed: %s\n", mysql_error(conn));
    //db_mysql_disconnect();
    return -1;
  }

  rc = get_column_data(res, 0, 0, &lastdate);
  if (rc < 0) {
    return -1;
  }

  printf("%s\n", lastdate);
  
  return 0;
}
