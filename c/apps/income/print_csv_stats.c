/*https://www.quora.com/How-do-I-read-a-csv-file-and-then-use-those-fields-to-create-an-array-structure-in-C*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csv_helper.h"
#include "file.h"
#include "stock.h"
#include "stats.h"

int main(int argc, char **argv) {
  double *close;
  int num_rows;
  double min_close;
  double max_close;

  if (argc != 3) {
    fputs("usage: print_csv_stats filename.csv SYMBOL\n", stderr);
    return 1;
  }

  load_close_prices(argv[1]);

  close = csv_close_prices();
  num_rows = csv_num_rows();

  stats_min(close, num_rows, &min_close);
  stats_max(close, num_rows, &max_close);

  printf("Min: %f\n", min_close);
  printf("Max: %f\n", max_close);

  return 0;
}
