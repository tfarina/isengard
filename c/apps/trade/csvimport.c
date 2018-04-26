/*https://www.quora.com/How-do-I-read-a-csv-file-and-then-use-those-fields-to-create-an-array-structure-in-C*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "csv.h"
#include "csv_helper.h"
#include "iniparser.h"
#include "db_mysql.h"
#include "env.h"
#include "file.h"
#include "stock.h"

#define USERCONFFILE ".traderc"

int main(int argc, char **argv) {
  char *homedir;
  char *userconffile;
  dictionary *ini;
  const char *host;
  const char *user;
  const char *password;
  const char *dbname;
  MYSQL *conn = NULL;
  size_t len;
  char *csvdata;
  struct csv_parser parser;
  int rc;
  stock_info_t stock;
  size_t bytes_processed;
  size_t i;

  homedir = get_homedir();
  userconffile = make_file_path(homedir, USERCONFFILE);
  ini = iniparser_load(userconffile);
  host = iniparser_getstring(ini, "mysql:host", NULL);
  user = iniparser_getstring(ini, "mysql:user", NULL);
  password = iniparser_getstring(ini, "mysql:password", NULL);
  dbname = iniparser_getstring(ini, "mysql:dbname", NULL);

  if (argc != 3) {
    fputs("usage: csvimport filename.csv symbol\n", stderr);
    return 1;
  }
 
  csvdata = readfile(argv[1], &len);
  if (csvdata == NULL)
    return 1;
 
  rc = csv_init(&parser, CSV_APPEND_NULL);
  if (rc != 0) {
    free(csvdata);
    fputs("failed to initialize CSV parser\n", stderr);
    return 1;
  }
 
  memset((void *)&stock, 0, sizeof(stock_info_t));
  stock.symbol = strdup(argv[2]);
  stock.ticks_alloc = 2;
  stock.ticks = malloc(stock.ticks_alloc * sizeof(stock_tick_t));
  if (stock.ticks == NULL) {
    fprintf(stderr, "failed to allocate %zu bytes for city data\n",
	    stock.ticks_alloc * sizeof(stock_tick_t));
    free(csvdata);
    return 1;
  }
 
  bytes_processed = csv_parse(&parser, (void*)csvdata, len,
                              process_field, process_row, &stock);
  rc = csv_fini(&parser, process_field, process_row, &stock);
  free(csvdata);
 
  if (stock.error || rc != 0 || bytes_processed < len) {
    fprintf(stderr,
            "read %zu bytes out of %zu: %s\n",
	    bytes_processed, len, csv_strerror(csv_error(&parser)));
    return 1;
  }
 
  if ((conn = db_mysql_connect(host, user, password, dbname)) == NULL) {
    return -1;
  }

  /* NOTE: This will overwrite existing data. */
  printf("Importing records...\n");

  for (i = 0; i < stock.ticks_used; i++) {
    stock_tick_t *tick = stock.ticks + i;

    if (stock_add_tick(conn, &stock, tick) != -1) {
    }

    printf("symbol=\"%s\"; date=\"%s\"; open=%.4lf; high=%.4lf; low=%.4lf; close=%.4lf; adj_close=%.4lf; volume=%d\n",
           stock.symbol, tick->date, tick->open, tick->high, tick->low, tick->close, tick->adj_close, tick->volume);

    free(tick->date);
  }

  printf("%d rows imported\n", stock.ticks_used);

  free(stock.ticks);
 
  return 0;
}
