/*https://www.quora.com/How-do-I-read-a-csv-file-and-then-use-those-fields-to-create-an-array-structure-in-C*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "third_party/libcsv/csv.h"
#include "csv_helper.h"
#include "file.h"
#include "stock.h"
#include "strutils.h"

int main(int argc, char **argv) {
  char *csvdata;
  size_t csvdata_len;
  int rc;
  struct csv_parser parser;
  stock_info_t stock;
  size_t bytes_processed;
  size_t i;

  if (argc != 3) {
    fputs("usage: ticks filename.csv SYMBOL\n", stderr);
    return 1;
  }

  csvdata = read_file(argv[1], &csvdata_len);
  if (csvdata == NULL) {
    return 1;
  }

  if (csv_init(&parser, CSV_APPEND_NULL) != 0) {
    free(csvdata);
    fprintf(stderr, "failed to initialize CSV parser\n");
    return 1;
  }
 
  memset((void *)&stock, 0, sizeof(stock_info_t));
  stock.symbol = f_strdup(argv[2]);
  stock_ticks_alloc(&stock, 2);
  if (stock.ticks == NULL) {
    fprintf(stderr, "failed to allocate %zu bytes for stock data\n",
	    stock.ticks_capacity * sizeof(stock_tick_t));
    free(csvdata);
    return 1;
  }
 
  bytes_processed = csv_parse(&parser, (void*)csvdata, csvdata_len,
                              csv_column_cb, csv_row_cb, &stock);
  rc = csv_fini(&parser, csv_column_cb, csv_row_cb, &stock);
  free(csvdata);
 
  if (stock.error || rc != 0 || bytes_processed < csvdata_len) {
    fprintf(stderr,
            "read %zu bytes out of %zu: %s\n",
	    bytes_processed, csvdata_len, csv_strerror(csv_error(&parser)));
    return 1;
  }

  csv_free(&parser);

  for (i = 0; i < stock.ticks_length; i++) {
    stock_tick_t *tick = stock.ticks + i;

    printf("%s,", tick->date);
    printf("%.4lf,", tick->open);
    printf("%.4lf,", tick->high);
    printf("%.4lf,", tick->low);
    printf("%.4lf,", tick->close);
    printf("%.4lf,", tick->adj_close);
    printf("%d\n", tick->volume);
  }

  free(stock.ticks);
 
  return 0;
}
