/*https://www.quora.com/How-do-I-read-a-csv-file-and-then-use-those-fields-to-create-an-array-structure-in-C*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "csv.h"
#include "iniparser.h"
#include "file.h"
#include "stock.h"

#define USERCONFFILE ".traderc"
#define PATH_SEP '/'

static char *get_homedir(void)
{
  return getenv("HOME");
}

/* Code from msmtp:src/tools.c:get_filename */
static char *make_file_path(const char *directory, const char *name)
{
  char *path;
  size_t dirlen;

  dirlen = strlen(directory);
  path = malloc((dirlen + strlen(name) + 2) * sizeof(char));
  strcpy(path, directory);
  if (dirlen == 0 || path[dirlen - 1] != PATH_SEP) {
    path[dirlen++] = PATH_SEP;
  }
  strcpy(path + dirlen, name);
  return path;
}

static MYSQL *db_mysql_connect(const char *host, const char *user,
                               const char *password, const char *dbname)
{
  MYSQL *conn;
  unsigned int port = 0;

  if ((conn = mysql_init(NULL)) == NULL) {
    fprintf(stderr, "mysql: unable to allocate memory for database connection.\n");
    return NULL;
  }

  if (mysql_real_connect(conn, host, user, password, dbname, port,
                         NULL, 0) == NULL) {
    fprintf(stderr, "mysql: connection to database failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return NULL;
  }

  return conn;
}

void process_field(void *field,
		   size_t field_len __attribute__((unused)),
		   void *ctx)
{
  stock_info_t *stock = (stock_info_t *)ctx;
  if (stock->error) return;
 
  stock_data_t *cur_tick = stock->ticks + stock->ticks_used;
 
  // used for parsing floating-point values:
  // (declaring these in a switch/case is annoying)
  char *endptr;
  double dval;
 
  switch (stock->cur_field) {
  case DATE:
    // start of a new record; check if we need to reallocate
    if (stock->ticks_used == stock->ticks_alloc) {
      stock->ticks_alloc *= 2;
      stock->ticks = realloc(stock->ticks,
			     sizeof(stock_data_t) * stock->ticks_alloc);
      if (stock->ticks == NULL) {
	fprintf(stderr,
		"failed to reallocate %zu bytes for stock data: ",
		sizeof(stock_data_t) * stock->ticks_alloc);
	perror(NULL);
	stock->error = 1;
	return;
      }
      cur_tick = stock->ticks + stock->ticks_used;
    }
 
    // anyway, we just got tick data
    cur_tick->date = strdup((char*)field);
    break;
 
  case OPEN:
  case HIGH:
  case LOW:
  case CLOSE:
  case ADJ_CLOSE:
    if (strcmp((char*)field, "null") != 0) {
      dval = strtod((char*)field, &endptr);
      if (*endptr != '\0') {
        fprintf(stderr,
                "non-float value in record %zu, field %u: \"%s\"\n",
                 stock->ticks_used + 1, stock->cur_field + 1, field);
        stock->error = 1;
        return;
      }
 
      if (stock->cur_field == OPEN)
        cur_tick->open = dval;
      else if (stock->cur_field == HIGH)
        cur_tick->high = dval;
      else if (stock->cur_field == LOW)
        cur_tick->low = dval;
      else if (stock->cur_field == CLOSE)
        cur_tick->close = dval;
      else if (stock->cur_field == ADJ_CLOSE)
        cur_tick->adj_close = dval;
    }
  case VOLUME:
    cur_tick->volume = atoi((char*)field);
  }
 

  if (stock->cur_field == VOLUME) stock->ticks_used++;
  stock->cur_field = (stock->cur_field + 1) % 7;
}
 
void process_row(int delim __attribute__((unused)), void *ctx) {
  stock_info_t *stock = (stock_info_t *)ctx;
  if (stock->error) return;
 
  if (stock->cur_field != DATE) {
    fprintf(stderr, "not enough fields in row %zu\n", stock->ticks_used);
    stock->error = 1;
  }
}

static int stock_add_tick(MYSQL *conn, const char *symbol, stock_data_t *tick)
{
  char query[256];
  MYSQL_RES *res;

  /* Determine if this is an insert or update operation. */
  sprintf(query, "SELECT symbol FROM historicaldata WHERE symbol = \"%s\" and date = \"%s\"", symbol, tick->date);
  if (mysql_query(conn, query)) {
    fprintf(stderr, "mysql: sql insert failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  res = mysql_store_result(conn);
  if (res == NULL) {
    fprintf(stderr, "mysql: mysql_store_result failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  /* If there is a record with this symbol and date, update it, otherwise insert new record. */
  if (mysql_num_rows(res)) {
    sprintf(query, "UPDATE historicaldata SET open=%f, high=%f, low=%f, close=%f, adjClose=%f, volume=%d WHERE symbol = \"%s\" and date = \"%s\"", tick->open, tick->high, tick->low, tick->close, tick->adj_close, tick->volume, symbol, tick->date);
  } else {
    sprintf(query, "INSERT INTO historicaldata (symbol, date, open, high, low, close, adjClose, volume) VALUES ('%s', '%s', %f, %f, %f, %f, %f, %d)",
            symbol, tick->date, tick->open, tick->high, tick->low, tick->close, tick->adj_close, tick->volume);
  }

  if (mysql_query(conn, query)) {
    fprintf(stderr, "mysql: sql operation failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  printf("%s\n", query);

  return 0;
}
 
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
  stock.ticks = malloc(stock.ticks_alloc * sizeof(stock_data_t));
  if (stock.ticks == NULL) {
    fprintf(stderr, "failed to allocate %zu bytes for city data\n",
	    stock.ticks_alloc * sizeof(stock_data_t));
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
    stock_data_t *tick = stock.ticks + i;

    if (stock_add_tick(conn, stock.symbol, tick) != -1) {
    }

    printf("symbol=\"%s\"; date=\"%s\"; open=%.4lf; high=%.4lf; low=%.4lf; close=%.4lf; adj_close=%.4lf; volume=%d\n",
           stock.symbol, tick->date, tick->open, tick->high, tick->low, tick->close, tick->adj_close, tick->volume);

    free(tick->date);
  }

  printf("%d rows imported\n", stock.ticks_used);

  free(stock.ticks);
 
  return 0;
}
