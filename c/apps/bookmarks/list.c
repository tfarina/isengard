/*
 * Modified from http://zetcode.com/db/mysqlc/
 */

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

static const char kDBHost[] = "localhost";
static const char kDBUser[] = "ken";
static const char kDBPassword[] = "194304";
static const char kDBName[] = "ctestdb";

static void finish_with_error(MYSQL *sql) {
  fprintf(stderr, "%s\n", mysql_error(sql));
  mysql_close(sql);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  MYSQL *conn = NULL;
  MYSQL_RES *res = NULL;
  unsigned int num_fields;
  MYSQL_ROW row;
  unsigned int port = 0;
  char query[256];
  size_t querylen;
  int i;

  if ((conn = mysql_init(NULL)) == NULL) {
    fprintf(stderr, "mysql_init() failed\n");
    exit(EXIT_FAILURE);
  }

  if (mysql_real_connect(conn, kDBHost, kDBUser, kDBPassword, kDBName, port,
                         NULL, 0) == NULL) {
    fprintf(stderr, "mysql: connection to database failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  querylen = snprintf(query, sizeof(query), "SELECT * FROM bookmarks");

  if (mysql_real_query(conn, query, querylen)) {
    finish_with_error(conn);
  }

  if ((res = mysql_store_result(conn)) == NULL) {
    finish_with_error(conn);
  }

  num_fields = mysql_num_fields(res);

  while ((row = mysql_fetch_row(res))) {
    for (i = 0; i < num_fields; i++) {
      printf("%s ", row[i] ? row[i] : "NULL");
    }
    printf("\n");
  }

  mysql_free_result(res);
  mysql_close(conn);

  return 0;
}
