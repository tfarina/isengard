#include "csv_helper.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "third_party/libcsv/csv.h"
#include "stock.h"
#include "strutils.h"

typedef enum {
  CSV_COLUMN_DATE,
  CSV_COLUMN_OPEN,
  CSV_COLUMN_HIGH,
  CSV_COLUMN_LOW,
  CSV_COLUMN_CLOSE,
  CSV_COLUMN_ADJ_CLOSE,
  CSV_COLUMN_VOLUME
} csv_column_t;

static int num_rows;
static unsigned colnum; /* current column - zero based index */

void csv_count_cb(int c, void *data) {
  num_rows++;
}

void csv_column_cb(void *field,
		   size_t field_len UNUSED,
		   void *ctx)
{
  stock_info_t *stock = (stock_info_t *)ctx;
  if (stock->error) return;

  stock_tick_t *cur_tick = stock->ticks + stock->ticks_length;
 
  // used for parsing floating-point values:
  // (declaring these in a switch/case is annoying)
  char *endptr;
  double dval;

  // Skip the first (header) line?
  if (strcmp((char*)field, "Date") == 0 ||
      strcmp((char*)field, "Open") == 0 ||
      strcmp((char*)field, "High") == 0 ||
      strcmp((char*)field, "Low") == 0 ||
      strcmp((char*)field, "Close") == 0 ||
      strcmp((char*)field, "Adj Close") == 0 ||
      strcmp((char*)field, "Volume") == 0) {
    return;
  }
 
  switch (colnum) {
  case CSV_COLUMN_DATE:
    /* Start of a new record; check if we need to reallocate. */
    /* If the number of entries in the vector has reached the capacity, resize. */
    if (stock->ticks_length == stock->ticks_capacity) {
      stock->ticks_capacity *= 2;
      stock->ticks = realloc(stock->ticks,
                             stock->ticks_capacity * sizeof(stock_tick_t));
      if (stock->ticks == NULL) {
        fprintf(stderr,
                "failed to reallocate %zu bytes for stock data: ",
		sizeof(stock_tick_t) * stock->ticks_capacity);
        perror(NULL);
        stock->error = 1;
	return;
      }
      cur_tick = stock->ticks + stock->ticks_length;
    }
 
    // anyway, we just got tick data
    cur_tick->date = f_strdup((char*)field);
    break;
 
  case CSV_COLUMN_OPEN:
  case CSV_COLUMN_HIGH:
  case CSV_COLUMN_LOW:
  case CSV_COLUMN_CLOSE:
  case CSV_COLUMN_ADJ_CLOSE:
    if (strcmp((char*)field, "null") != 0) {
      dval = strtod((char*)field, &endptr);
      if (*endptr != '\0') {
        fprintf(stderr,
                "non-float value in record %zu, field %u: \"%s\"\n",
                 stock->ticks_length + 1, colnum + 1, field);
        stock->error = 1;
        return;
      }
 
      if (colnum == CSV_COLUMN_OPEN)
        cur_tick->open = dval;
      else if (colnum == CSV_COLUMN_HIGH)
        cur_tick->high = dval;
      else if (colnum == CSV_COLUMN_LOW)
        cur_tick->low = dval;
      else if (colnum == CSV_COLUMN_CLOSE)
        cur_tick->close = dval;
      else if (colnum == CSV_COLUMN_ADJ_CLOSE)
        cur_tick->adj_close = dval;
    }
  case CSV_COLUMN_VOLUME:
    cur_tick->volume = atoi((char*)field);
  }
 

  if (colnum == CSV_COLUMN_VOLUME) {
    stock->ticks_length++;
  }

  colnum = (colnum + 1) % 7;
}
 
void csv_row_cb(int delim UNUSED, void *ctx) {
  stock_info_t *stock = (stock_info_t *)ctx;
  if (stock->error) return;
 
  if (colnum != CSV_COLUMN_DATE) {
    fprintf(stderr, "not enough fields in row %zu\n", stock->ticks_length);
    stock->error = 1;
  }

  colnum = 0;
}

int csv_num_rows(void) {
  return num_rows;
}

static double *close;

/*static long unsigned colnum;*/
static long unsigned rownum;

void csv_column_cb1(void *p1, size_t s, void *p2) {
  char *endptr;
  double dval;

  if (rownum == 0) {
    return;
  }

  printf("%u, ", colnum);
  switch (colnum++) {
  case CSV_COLUMN_CLOSE:
    printf(" %s ", (char *)p1);
    dval = strtod((char*)p1, &endptr);
    close[rownum] = dval;
    break;
  }
}

void csv_row_cb2(int c, void *data) {
  printf("ROW: %lu\n", rownum);

  colnum = 0;
  rownum = rownum + 1;
}

int load_close_prices(char const *filename) {
  struct csv_parser parser;
  FILE *fp;
  char buf[1024];
  size_t bytes_read;
  int i;
  int num_rows;

  if (csv_init(&parser, 0) != 0) {
    fprintf(stderr, "failed to initialize csv parser\n");
    return -11;
  }

  fp = fopen(filename, "rb");
  if (fp == NULL) {
    fprintf(stderr, "failed to open file %s: %s\n", filename, strerror(errno));
    csv_free(&parser);
    return -1;
  }

  while ((bytes_read = fread(buf, 1, 1024, fp)) > 0) {
    if (csv_parse(&parser, buf, bytes_read, NULL, csv_count_cb, NULL) != bytes_read) {
      fprintf(stderr, "error while parsing file: %s\n", csv_strerror(csv_error(&parser)));
      csv_free(&parser);
      return -1;
    }
  }

  csv_fini(&parser, NULL, csv_count_cb, NULL);
  csv_free(&parser);

  if (ferror(fp)) {
    fprintf(stderr, "error reading file %s\n", filename);
    fclose(fp);
    return -1;
  }

  fclose(fp);

  num_rows = csv_num_rows();

  printf("Found %d rows\n", num_rows);

  /* Now do a second parse to read the close prices into the array. */ 
  close = malloc(num_rows * sizeof(double));

  if (csv_init(&parser, 0) != 0) {
    fprintf(stderr, "failed to initialize csv parser\n");
    return -1;
  }

  fp = fopen(filename, "rb");
  if (fp == NULL) {
    fprintf(stderr, "failed to open file %s: %s\n", filename, strerror(errno));
    csv_free(&parser);
    return -1;
  }

  while ((bytes_read = fread(buf, 1, 1024, fp)) > 0) {
    if (csv_parse(&parser, buf, bytes_read, csv_column_cb1, csv_row_cb2, NULL) != bytes_read) {
      fprintf(stderr, "error while parsing file: %s\n", csv_strerror(csv_error(&parser)));
      csv_free(&parser);
      return -1;
    }
  }

  csv_fini(&parser, csv_column_cb1, csv_row_cb2, NULL);
  csv_free(&parser);

  if (ferror(fp)) {
    fprintf(stderr, "error reading file %s\n", filename);
    fclose(fp);
    return -1;
  }

  fclose(fp);

  for (i = 0; i < num_rows; i++) {
    printf("%f\n", close[i]);
  }

  return 0;
}

double *csv_close_prices(void) {
  return close;
}
