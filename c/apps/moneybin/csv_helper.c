#include "csv_helper.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "third_party/libcsv/csv.h"

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

typedef enum return_code_e {
  RC_OK,
  RC_ERROR,
} return_code_t;

static char *parse_str(char const *field, size_t length, return_code_t *rc) {
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

static double parse_price(char const *field, size_t length, return_code_t *rc) {
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

void csv_count_cb(int c, void *data) {
  num_rows++;
}
 
void csv_row_cb(int delim UNUSED, void *data) {
  colnum = 0;
}

int csv_num_rows(void) {
  return num_rows;
}

static int ignore_first_line = 1;
static long unsigned currentrow;
static long unsigned currentcolumn;
static double *close;

void csv_column_cb1(void *buffer, size_t len, void *data) {
  char *endptr;
  double dval;

  if (ignore_first_line) {
    return;
  }

  printf("%lu, ", currentcolumn);

  switch (currentcolumn++) {
  case CSV_COLUMN_CLOSE:
    printf(" %s ", (char *)buffer);

    dval = strtod((char*)buffer, &endptr);
    close[currentrow] = dval;

    break;
  }
}

void csv_row_cb2(int c, void *data) {
  if (ignore_first_line) {
    ignore_first_line = 0;
  } else {
    printf("ROW: %lu\n", currentrow);

    /* We got a row, increment the counter. */
    currentrow = currentrow + 1;

    /* We got a row, reset the index of the column counter. */
    currentcolumn = 0;
  }
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
    return -1;
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
