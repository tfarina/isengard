/*https://www.quora.com/How-do-I-read-a-csv-file-and-then-use-those-fields-to-create-an-array-structure-in-C*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "csv.h"
#include "csv_helper.h"
#include "file.h"
#include "stock.h"

static long unsigned num_rows;
static double *close;

static long unsigned colnum;
static long unsigned rownum;

static void csv_count_cb(int c, void *data) {
  num_rows++;
}

static void csv_column_cb1(void *p1, size_t s, void *p2) {
  char *endptr;
  double dval;

  if (rownum == 0) {
    return;
  }

  printf("%lu, ", colnum);
  switch (colnum++) {
  case 4 /*Close*/:
    printf(" %s ", (char *)p1);
    dval = strtod((char*)p1, &endptr);
    close[rownum] = dval;
    break;
  }
}

static void csv_row_cb2(int c, void *data) {
  printf("ROW: %lu\n", rownum);

  colnum = 0;
  rownum = rownum + 1;
}

int main(int argc, char **argv) {
  struct csv_parser parser;
  FILE *fp;
  char buf[1024];
  size_t bytes_read;
  long unsigned i;

  if (argc != 3) {
    fputs("usage: print_csv_stats filename.csv SYMBOL\n", stderr);
    return 1;
  }

  if (csv_init(&parser, 0) != 0) {
    fprintf(stderr, "failed to initialize csv parser\n");
    return -11;
  }

  fp = fopen(argv[1], "rb");
  if (fp == NULL) {
    fprintf(stderr, "failed to open file %s: %s\n", argv[1], strerror(errno));
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
    fprintf(stderr, "error reading file %s\n", argv[1]);
    fclose(fp);
    return -1;
  }

  fclose(fp);

  printf("Found %lu rows\n", num_rows);

  /* Now do a second parse to read the close prices into the array. */ 
  close = malloc(num_rows * sizeof(double));

  if (csv_init(&parser, 0) != 0) {
    fprintf(stderr, "failed to initialize csv parser\n");
    return -11;
  }

  fp = fopen(argv[1], "rb");
  if (fp == NULL) {
    fprintf(stderr, "failed to open file %s: %s\n", argv[1], strerror(errno));
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
    fprintf(stderr, "error reading file %s\n", argv[1]);
    fclose(fp);
    return -1;
  }

  fclose(fp);

  for (i = 0; i < num_rows; i++) {
    printf("%f\n", close[i]);
  }

  return 0;
}
