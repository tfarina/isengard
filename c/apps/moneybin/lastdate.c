#include <stdio.h>
#include <string.h>

#include "db.h"
#include "driver_mysql.h"

int main(void)
{
  db_config_t config;
  MYSQL *conn;
  int rc;
  char query[256];
  MYSQL_RES *res = NULL;
  size_t num_rows;
  MYSQL_ROW row;

  db_config_init(&config);

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

  if ((res = mysql_store_result(conn)) == NULL) {
    fprintf(stderr, "mysql: sql result retrieval failed: %s\n", mysql_error(conn));
    //db_mysql_disconnect();
    return -1;
  }

  num_rows =  mysql_num_rows(res);
  if (num_rows <= 0) {
    fprintf(stderr, "No rows found\n");
    return -1;
  }

  mysql_data_seek(res, 0);
  row = mysql_fetch_row(res);
  if (row == NULL) {
    fprintf(stderr, "No row found\n");
    return -1;
  }
  printf("%s\n", row[0]);
  
  return 0;
}
