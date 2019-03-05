/*
 * Modified from http://zetcode.com/db/mysqlc/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "db.h"

static MYSQL *conn = NULL;

static int db_mysql_connect(const char *host, int unsigned port, const char *user,
                            const char *password, const char *dbname)
{
  if ((conn = mysql_init(NULL)) == NULL) {
    fprintf(stderr, "mysql: unable to allocate memory for database connection.\n");
    return -1;
  }

  if (mysql_real_connect(conn, host, user, password, dbname, port,
                         NULL, 0) == NULL) {
    fprintf(stderr, "mysql: connection to database failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  return 0;
}

static int db_mysql_disconnect(void)
{
  mysql_close(conn);
  conn = NULL;
  return 0;
}

/*
 * http://www.kitebird.com/mysql-book/ch06-3ed.pdf
 */
static void print_dashes(MYSQL_RES *res)
{
  MYSQL_FIELD *field;
  unsigned int i, j;
  unsigned int num_fields;

  mysql_field_seek(res, 0);

  fputc('+', stdout);

  num_fields = mysql_num_fields(res);

  for (i = 0; i < num_fields; i++) {
    field = mysql_fetch_field(res);
    for (j = 0; j < field->max_length +2; j++) {
      fputc('-', stdout);
    }
    fputc('+', stdout);
  }

  fputc('\n', stdout);
}

static int ab_print_contact_records(void)
{
  char query[256];
  MYSQL_RES *res = NULL;
  unsigned int num_fields;
  unsigned long col_len;
  MYSQL_FIELD *field;
  MYSQL_ROW row;
  int i;

  sprintf(query, "SELECT * FROM contacts");

  if (mysql_query(conn, query)) {
    fprintf(stderr, "mysql: sql select failed: %s\n", mysql_error(conn));
    db_mysql_disconnect();
    return -1;
  }

  if ((res = mysql_store_result(conn)) == NULL) {
    fprintf(stderr, "mysql: sql result retrieval failed: %s\n", mysql_error(conn));
    db_mysql_disconnect();
    return -1;
  }

  num_fields = mysql_num_fields(res);

  /*
   * Determine column display widths -- requires result set to be
   * generated with mysql_store_result(), not mysql_use_result().
   */
  mysql_field_seek (res, 0);
  for (i = 0; i < num_fields; i++) {
    field = mysql_fetch_field (res);
    col_len = strlen (field->name);
    if (col_len < field->max_length)
      col_len = field->max_length;
    if (col_len < 4 && !IS_NOT_NULL (field->flags))
      col_len = 4;    /* 4 = length of the word "NULL" */
    field->max_length = col_len;    /* reset column info */
  }

  print_dashes(res);
  fputc('|', stdout);
  mysql_field_seek(res, 0);
  for(i = 0; i < num_fields; i++) {
    field = mysql_fetch_field(res);
    printf(" %-*s |", (int)field->max_length, field->name);
  }
  fputc('\n', stdout);
  print_dashes(res);

  while ((row = mysql_fetch_row(res))) {
    mysql_field_seek(res, 0);
    fputc('|', stdout);
    for (i = 0; i < num_fields; i++) {
      field = mysql_fetch_field(res);
      if (row[i] == NULL) {
        printf(" %-*s |", (int)field->max_length, "NULL");
      } else if(IS_NUM(field->type)) {
        printf(" %*s |", (int)field->max_length, row[i]);
      } else {
        printf(" %-*s |", (int)field->max_length, row[i]);
      }
    }
    fputc('\n', stdout);
  }

  print_dashes(res);

  mysql_free_result(res);

  return 0;
}

int main(int argc, char **argv) {
  db_config_t config;

  db_config_init(&config);

  if (db_mysql_connect(config.host, config.port, config.user, config.password, config.dbname)) {
    return -1;
  }

  if (ab_print_contact_records()) {
    return -1;
  }

  db_mysql_disconnect();

  return 0;
}
