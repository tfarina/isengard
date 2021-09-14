#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ta.h"
#include "third_party/libcsv/csv.h"
#include "timestamp.h"

typedef struct csv_counts_s {
  long unsigned fields;
  long unsigned rows;
} csv_counts_t;

typedef struct csv_state_s {
  int ignore_first_line;

  /**
   * Which line (row) we are in the parsing process.
   */
  int row;

  /**
   * The next field (column number) to parse.
   */
  int column;

  ta_bars_t *bars;
} csv_state_t;

/**
 * The columns in the yahoo CSV file.
 */
typedef enum csv_column_e {
  CSV_COLUMN_DATE,
  CSV_COLUMN_OPEN,
  CSV_COLUMN_HIGH,
  CSV_COLUMN_LOW,
  CSV_COLUMN_CLOSE,
  CSV_COLUMN_ADJ_CLOSE,
  CSV_COLUMN_VOLUME
} csv_column_t;

static char *parse_str(char const *field, size_t length, int *rc) {
  if (length > 0) {
    char *str = (char *)malloc((length + 1) * sizeof(char));
    strncpy(str, field, length + 1);

    *rc = TA_SUCCESS;
    return str;
  }

  *rc = TA_FAILURE;
  return NULL;
}

static void parse_iso8601_date(char const *field, timestamp_t *timestamp) {
  int year, month, day;

  year  = atoi(field);
  month = atoi(field + 5);
  day   = atoi(field + 8);

  set_date(year, month, day, timestamp);
}

static double parse_price(char const *field, size_t length, int *rc) {
  char *endptr;
  double price;

  price = (double)strtod(field, &endptr);
  if (length > 0 && (endptr == NULL || *endptr == '\0')) {
    *rc = TA_SUCCESS;
    return price;
  }

  *rc = TA_FAILURE;
  return -1.0f;
}

static timestamp_t *new_timestamp_from_string(char const *date_string) {
  timestamp_t *timestamp;

  timestamp = malloc(sizeof(*timestamp));

  parse_iso8601_date(date_string, timestamp);

  return timestamp;
}

static void csv_field_count_cb(void *field, size_t field_length, void *data) {
  csv_counts_t *cnt = (csv_counts_t *)data;

  cnt->fields++;
}

static void csv_row_count_cb(int c, void *data) {
  csv_counts_t *cnt = (csv_counts_t *)data;

  cnt->rows++;
}

/**
 * This functions is called each time a new field has been found.
 */
static void csv_read_field_cb(void *field, size_t field_length, void *data) {
  csv_state_t *state = (csv_state_t *)data;
  char *buffer;
  int rc;
  timestamp_t *timestamp;

  if (state->ignore_first_line) {
    return;
  }

  buffer = (char *)malloc((field_length + 1) * sizeof(char));
  strncpy(buffer, field, field_length);
  buffer[field_length] = '\0';

  switch (state->column) {
  case CSV_COLUMN_DATE:
    timestamp = new_timestamp_from_string(buffer);
    memcpy(state->bars->timestamp + state->row, timestamp, sizeof(timestamp_t));
    break;
  case CSV_COLUMN_OPEN:
    state->bars->open[state->row] = parse_price(buffer, field_length, &rc);
    break;
  case CSV_COLUMN_HIGH:
    state->bars->high[state->row] = parse_price(buffer, field_length, &rc);
    break;
  case CSV_COLUMN_LOW:
    state->bars->low[state->row] = parse_price(buffer, field_length, &rc);
    break;
  case CSV_COLUMN_CLOSE:
    state->bars->close[state->row] = parse_price(buffer, field_length, &rc);
    break;
  case CSV_COLUMN_ADJ_CLOSE:
    state->bars->adjclose[state->row] = parse_price(buffer, field_length, &rc);
    break;
  case CSV_COLUMN_VOLUME:
    state->bars->volume[state->row] = atoi(buffer);
    break;
  default:
    rc = TA_FAILURE;
    break;
  }

  free(buffer);

  state->column++;
}

/**
 * This functions is called each time a new row has been found.
 */
static void csv_read_row_cb(int c, void *data) {
  csv_state_t *state = (csv_state_t *)data;

  if (state->ignore_first_line) {
    state->ignore_first_line = 0;
    return;
  }

  state->row++;
  state->column = 0;
}

int read_csv(char const *filename, ta_bars_t **outbars) {
  FILE* fp;
  struct csv_parser parser;
  char buf[1024];
  size_t bytes_read;
  csv_counts_t c;
  csv_state_t *state;
  ta_bars_t *bars;
  int i;

  c.fields = 0;
  c.rows = 0;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open file %s: %s\n", filename, strerror(errno));
    return -1;
  }

 if (csv_init(&parser, CSV_STRICT | CSV_REPALL_NL | CSV_APPEND_NULL | CSV_STRICT_FINI) != 0) {
    fprintf(stderr, "Failed to initialize csv parser\n");
    return -1;
  }

  /* First we are going to read the whole csv file to gather the total
   * number of rows and fields.
   */
  while ((bytes_read = fread(buf, sizeof(char), sizeof(buf), fp)) > 0) {
    if (csv_parse(&parser, buf, bytes_read, csv_field_count_cb, csv_row_count_cb, &c) != bytes_read) {
      fprintf(stderr, "Error while parsing %s: %s\n", filename, csv_strerror(csv_error(&parser)));
      csv_free(&parser);
      fclose(fp);
      return -1;
    }
  }

  csv_fini(&parser, csv_field_count_cb, csv_row_count_cb, &c);

#if 0
  printf(" %lu rows", c.rows);
  printf(" %lu fields", c.fields);
  printf(" %s", filename);
  putchar('\n');
#endif

  state = malloc(sizeof(*state));
  state->ignore_first_line = 1;
  state->row = 0;
  state->column = 0;

  bars = malloc(sizeof(*bars));

  bars->numrows = c.rows - (state->ignore_first_line ? 1 : 0);

  bars->timestamp = malloc(sizeof(timestamp_t) * bars->numrows);
  bars->open = malloc(sizeof(double) * bars->numrows);
  bars->high = malloc(sizeof(double) * bars->numrows);
  bars->low = malloc(sizeof(double) * bars->numrows);
  bars->close = malloc(sizeof(double) * bars->numrows);
  bars->adjclose = malloc(sizeof(double) * bars->numrows);
  bars->volume = malloc(sizeof(int) * bars->numrows);

  state->bars = bars;

  if (fp) {
    fseek(fp, 0, SEEK_SET);
  }

  while ((bytes_read = fread(buf, sizeof(char), sizeof(buf), fp)) > 0) {
    if (csv_parse(&parser, buf, bytes_read, csv_read_field_cb, csv_read_row_cb, state) != bytes_read) {
      fprintf(stderr, "Error while parsing file %s: %s\n", filename, csv_strerror(csv_error(&parser)));
      csv_free(&parser);
      fclose(fp);
      return -1;
    }
  }

  csv_fini(&parser, csv_read_field_cb, csv_read_row_cb, state);
  csv_free(&parser);

  if (ferror(fp)) {
    fprintf(stderr, "Error reading file %s\n", filename);
    fclose(fp);
    return -1;
  }

  fclose(fp);

  *outbars = state->bars;

  return 0;
}
