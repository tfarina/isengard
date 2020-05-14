#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fstrutils.h"
#include "ta.h"

static void print_matrix(ta_bars_t *b) {
  int i;

  for (i = 0; i < b->numrows; i++) {
    printf("%9.3f ", b->open[i]);
    printf("%9.3f ", b->high[i]);
    printf("%9.3f ", b->low[i]);
    printf("%9.3f ", b->close[i]);
    printf("%9.3f ", b->adjclose[i]);
    printf("%d ", b->volume[i]);
    putc('\n', stdout);
  }
}

int main(int argc, char **argv) {
  int err;
  char *filename;
  ta_bars_t *bars;

  if (argc != 2) {
    fputs("usage: csvcat filename.csv\n", stderr);
    return 1;
  }

  filename = f_strdup(argv[1]);

  err = read_csv(filename, &bars);
  if (err < 0) {
    return err;
  }

  print_matrix(bars);

  free(filename);

  return 0;
}
