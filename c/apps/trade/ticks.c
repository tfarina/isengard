/*https://www.quora.com/How-do-I-read-a-csv-file-and-then-use-those-fields-to-create-an-array-structure-in-C*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csv.h"
#include "csv_helper.h"
#include "file.h"
#include "stock.h"

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

  csvdata = readfile(argv[1], &csvdata_len);
  if (csvdata == NULL) {
    return 1;
  }

  rc = csv_init(&parser, CSV_APPEND_NULL);
  if (rc != 0) {
    free(csvdata);
    fputs("failed to initialize CSV parser\n", stderr);
    return 1;
  }
 
  memset((void *)&stock, 0, sizeof(stock_info_t));
  stock.symbol = strdup(argv[2]);
  stock.ticks_capacity = 2;
  stock.ticks = malloc(stock.ticks_capacity * sizeof(stock_tick_t));
  if (stock.ticks == NULL) {
    fprintf(stderr, "failed to allocate %zu bytes for stock data\n",
	    stock.ticks_capacity * sizeof(stock_tick_t));
    free(csvdata);
    return 1;
  }
 
  bytes_processed = csv_parse(&parser, (void*)csvdata, csvdata_len,
                              process_field, process_row, &stock);
  rc = csv_fini(&parser, process_field, process_row, &stock);
  free(csvdata);
 
  if (stock.error || rc != 0 || bytes_processed < csvdata_len) {
    fprintf(stderr,
            "read %zu bytes out of %zu: %s\n",
	    bytes_processed, csvdata_len, csv_strerror(csv_error(&parser)));
    return 1;
  }
 
  printf("%s ticks:\n", stock.symbol);

  for (i = 0; i < stock.ticks_length; i++) {
    stock_tick_t *tick = stock.ticks + i;
    printf("date=\"%s\"; open=%.4lf; high=%.4lf; low=%.4lf; close=%.4lf; adj_close=%.4lf; volume=%d\n",
           tick->date, tick->open, tick->high, tick->low, tick->close, tick->adj_close, tick->volume);
    free(tick->date);
  }
  free(stock.ticks);
 
  return 0;
}
