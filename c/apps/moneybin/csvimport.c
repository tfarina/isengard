/*https://www.quora.com/How-do-I-read-a-csv-file-and-then-use-those-fields-to-create-an-array-structure-in-C*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "third_party/libcsv/csv.h"
#include "csv_helper.h"
#include "third_party/iniparser/iniparser.h"
#include "db.h"
#include "db_mysql.h"
#include "file.h"
#include "ffileutils.h"
#include "futils.h"
#include "stock.h"
#include "strutils.h"

static void csv_quote_column_cb(void *buffer, size_t len, void *data) {
}

static void csv_quote_row_cb(int c, void *data) {
}

static void csv_read_quotes(char const *filename) {
  FILE* fp;
  struct csv_parser parser;
  char buf[1024];
  size_t bytes_read;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    return;
  }

  while ((bytes_read = fread(buf, sizeof(char), sizeof(buf), fp)) > 0) {
    if (csv_parse(&parser, buf, bytes_read, csv_quote_column_cb, csv_quote_row_cb, NULL) != bytes_read) {
      fprintf(stderr, "Error while parsing file: %s\n", csv_strerror(csv_error(&parser)));
    }
  }

  fclose(fp);
}

static int quote_exists(MYSQL *conn, char const *symbol, stock_tick_t *tick)
{
  char query[256];
  MYSQL_RES *res;

  /* Determine if this is an insert or update operation. */
  sprintf(query, "SELECT symbol FROM historicaldata WHERE symbol = \"%s\" and date = \"%s\"", symbol, tick->date);
  if (mysql_query(conn, query)) {
    fprintf(stderr, "mysql: select query failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return 0;
  }

  res = mysql_store_result(conn);
  if (res == NULL) {
    fprintf(stderr, "mysql: mysql_store_result failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return 0;
  }

  return mysql_num_rows(res) > 0;
}

static int quote_update(MYSQL *conn, char const *symbol, stock_tick_t *tick)
{
  char query[256];

  sprintf(query, "UPDATE historicaldata SET open=%f, high=%f, low=%f, close=%f, adjClose=%f, volume=%d WHERE symbol = \"%s\" and date = \"%s\"",
          tick->open, tick->high, tick->low, tick->close, tick->adj_close, tick->volume, symbol, tick->date);

  if (mysql_query(conn, query)) {
    fprintf(stderr, "mysql: sql operation failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  return 0;
}

static int quote_insert(MYSQL *conn, char const *symbol, stock_tick_t *tick)
{
  char query[256];

  sprintf(query, "INSERT INTO historicaldata (symbol, date, open, high, low, close, adjClose, volume) VALUES ('%s', '%s', %f, %f, %f, %f, %f, %d)",
          symbol, tick->date, tick->open, tick->high, tick->low, tick->close, tick->adj_close, tick->volume);

  if (mysql_query(conn, query)) {
    fprintf(stderr, "mysql: sql operation failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  return 0;
}

int main(int argc, char **argv) {
  char *filename;
  char *symbol;
  size_t len;
  char *csvdata;
  struct csv_parser parser;
  int rc;
  stock_info_t stock;
  size_t bytes_processed;
  db_config_t config;
  MYSQL *conn = NULL;
  size_t i;

  if (argc != 3) {
    fputs("usage: csvimport filename.csv symbol\n", stderr);
    return 1;
  }

  filename = f_strdup(argv[1]);
  symbol = f_strdup(argv[2]);

  rc = read_file(filename, &csvdata, &len);
  if (rc < 0)
    return 1;
 
  memset((void *)&stock, 0, sizeof(stock_info_t));
  stock.symbol = symbol;
  stock_ticks_alloc(&stock, 2);
  if (stock.ticks == NULL) {
    fprintf(stderr, "failed to allocate %zu bytes for stock data\n",
	    stock.ticks_capacity * sizeof(stock_tick_t));
    free(csvdata);
    return 1;
  }
 
  if (csv_init(&parser, CSV_STRICT | CSV_APPEND_NULL) != 0) {
    free(csvdata);
    fprintf(stderr, "failed to initialize CSV parser\n");
    return 1;
  }

  bytes_processed = csv_parse(&parser, (void*)csvdata, len,
                              csv_column_cb, csv_row_cb, &stock);
  rc = csv_fini(&parser, csv_column_cb, csv_row_cb, &stock);
  free(csvdata);
 
  if (stock.error || rc != 0 || bytes_processed < len) {
    fprintf(stderr,
            "read %zu bytes out of %zu: %s\n",
	    bytes_processed, len, csv_strerror(csv_error(&parser)));
    return 1;
  }

  db_config_init(&config);

  rc = db_mysql_connect(&conn, config.host, config.user, config.password, config.dbname);
  if (rc < 0) {
    return -1;
  }

  /* NOTE: This will overwrite existing data. */
  printf("Importing records...\n");
  exit(1);

  for (i = 0; i < stock.ticks_length; i++) {
    stock_tick_t *tick = stock.ticks + i;

    if (stock_add_tick(conn, &stock, tick) != -1) {
    }

    printf("date=\"%s\"; open=%.4lf; high=%.4lf; low=%.4lf; close=%.4lf; adj_close=%.4lf; volume=%d\n",
           tick->date, tick->open, tick->high, tick->low, tick->close, tick->adj_close, tick->volume);
  }

  printf("%d rows imported\n", stock.ticks_length);

  free(stock.ticks);
 
  return 0;
}
