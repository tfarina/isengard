#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "db.h"
#include "db_mysql.h"
#include "stock.h"
#include "strutils.h"
#include "vector.h"
#include "third_party/iniparser/iniparser.h"
#include "third_party/libcsv/csv.h"

typedef enum result_code_e {
  RC_OK,
  RC_ERROR,
} result_code_t;

typedef struct csv_state_s {
  /**
   * The next field (column number) to parse. */
  int field;

  /**
   * Determines wheter there has been any error during parsing.
   */
  int any_error;

  /**
   * Data of the tick that we are currently parsing.
   */
  stock_tick_t cur_tick;

  /**
   * List of ticks parsed so far.
   */
  vector_t *ticks;
} csv_state_t;

typedef enum csv_column_e {
  CSV_COLUMN_DATE,
  CSV_COLUMN_OPEN,
  CSV_COLUMN_HIGH,
  CSV_COLUMN_LOW,
  CSV_COLUMN_CLOSE,
  CSV_COLUMN_ADJ_CLOSE,
  CSV_COLUMN_VOLUME
} csv_column_t;

static int ignore_first_line = 1;

static char *parse_str(char const *field, size_t length, result_code_t *rc) {
  if (length > 0) {
    char *str = (char *)malloc((length + 1) * sizeof(char));
    strncpy(str, field, length + 1);

    *rc = RC_OK;
    return str;
  } else {
    *rc = RC_ERROR;
    return NULL;
  }
}

static double parse_price(char const *field, size_t length, result_code_t *rc) {
  char *endptr;
  double price;

  price = (double)strtod(field, &endptr);
  if (length > 0 && (endptr == NULL || *endptr == '\0')) {
    *rc = RC_OK;
    return price;
  }

  *rc = RC_ERROR;
  return -1.0f;
}

/**
 * This functions is called each time a new csv field has been found.
 * It is responsible for determining which field is being parsed, and
 * updating the current_tick object.
 */
static void csv_new_field_cb(void *field, size_t field_length, void *data) {
  csv_state_t *state = (csv_state_t *)data;
  char *buffer;
  result_code_t rc;

  if (state->field == -1) {
    return;
  }

  if (ignore_first_line) {
    return;
  }

  buffer = (char *)malloc((field_length + 1) * sizeof(char));
  strncpy(buffer, field, field_length);
  buffer[field_length] = '\0';

  switch (state->field) {
  case CSV_COLUMN_DATE:
    state->cur_tick.date = parse_str(buffer, field_length, &rc);
    break;
  case CSV_COLUMN_OPEN:
    state->cur_tick.open = parse_price(buffer, field_length, &rc);
    break;
  case CSV_COLUMN_HIGH:
    state->cur_tick.high = parse_price(buffer, field_length, &rc);
    break;
  case CSV_COLUMN_LOW:
    state->cur_tick.low = parse_price(buffer, field_length, &rc);
    break;
  case CSV_COLUMN_CLOSE:
    state->cur_tick.close = parse_price(buffer, field_length, &rc);
    break;
  case CSV_COLUMN_ADJ_CLOSE:
    state->cur_tick.adj_close = parse_price(buffer, field_length, &rc);
    break;
  case CSV_COLUMN_VOLUME:
    state->cur_tick.volume = atoi(buffer);
    break;
  default:
    rc = RC_ERROR;
    break;
  }

  free(buffer);

  if (rc == RC_OK) {
    state->field++;
  } else {
    state->any_error = 1;
    state->field = -1;
  }
}

/**
 * This functions is called each time a new row has been found.
 */
static void csv_new_row_cb(int c, void *data) {
  csv_state_t *state = (csv_state_t *)data;

  if (ignore_first_line) {
    ignore_first_line = 0;
    return;
  }

  if (state->field != -1) {
    /**
     * This makes a copy of cur_tick, and smells like a hack. But that
     * is because 'vector_push_back' does not make a copy and the lifetime
     * of cur_tick is short, so it does not outlive the call to
     * csv_read_quotes and without a copy we end up with pointers pointing
     * to garbage ticks.
     */
    stock_tick_t *copy = malloc(sizeof(stock_tick_t));
    copy->date = state->cur_tick.date;
    copy->open = state->cur_tick.open;
    copy->high = state->cur_tick.high;
    copy->low = state->cur_tick.low;
    copy->close = state->cur_tick.close;
    copy->adj_close = state->cur_tick.adj_close;
    copy->volume = state->cur_tick.volume;
    vector_push_back(state->ticks, copy);
  }

  state->field = 0;
}

static void csv_read_quotes(char const *filename, vector_t *ticks) {
  FILE* fp;
  struct csv_parser parser;
  char buf[1024];
  size_t bytes_read;
  csv_state_t state;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open file %s: %s\n", filename, strerror(errno));
    return;
  }

  memset(&state, 0, sizeof(csv_state_t));
  state.ticks = ticks;

  if (csv_init(&parser, CSV_STRICT | CSV_APPEND_NULL | CSV_STRICT_FINI) != 0) {
    fprintf(stderr, "Failed to initialize csv parser\n");
    return;
  }

  while ((bytes_read = fread(buf, sizeof(char), sizeof(buf), fp)) > 0) {
    if (csv_parse(&parser, buf, bytes_read, csv_new_field_cb, csv_new_row_cb, &state) != bytes_read) {
      fprintf(stderr, "Error while parsing %s: %s\n", filename, csv_strerror(csv_error(&parser)));
    }
  }

  csv_fini(&parser, csv_new_field_cb, csv_new_row_cb, &state);
  csv_free(&parser);

  if (ferror(fp)) {
    fprintf(stderr, "error reading file %s\n", filename);
    fclose(fp);
    return;
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
  vector_t *ticks;
  int rc;
  db_config_t config;
  MYSQL *conn = NULL;
  size_t i;
  int num_updates = 0;
  int num_inserts = 0;

  if (argc != 3) {
    fputs("usage: csvimport filename.csv symbol\n", stderr);
    return 1;
  }

  filename = f_strdup(argv[1]);
  symbol = f_strdup(argv[2]);

  ticks = vector_alloc(256);

  csv_read_quotes(filename, ticks);

  db_config_init(&config);

  rc = db_mysql_connect(&conn, config.host, config.user, config.password, config.dbname);
  if (rc < 0) {
    return -1;
  }

  printf("Importing records...\n");

  for (i = 0; i < vector_size(ticks); i++) {
    stock_tick_t *tick = vector_get(ticks, i);

    if (quote_exists(conn, symbol, tick)) {
      quote_update(conn, symbol, tick);
      num_updates++;
    } else {
      quote_insert(conn, symbol, tick);
      num_inserts++;
    }

    printf("date=\"%s\"; open=%.4lf; high=%.4lf; low=%.4lf; close=%.4lf; adj_close=%.4lf; volume=%d\n",
           tick->date, tick->open, tick->high, tick->low, tick->close, tick->adj_close, tick->volume);
  }

  printf("%d rows updated\n", num_updates);
  printf("%d rows inserted\n", num_inserts);

  vector_free(ticks);
  free(filename);
  free(symbol);
 
  return 0;
}
