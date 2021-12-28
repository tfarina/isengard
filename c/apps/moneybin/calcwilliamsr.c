#include <stdlib.h>
#include <stdio.h>

#include "ta.h"

int main(int argc, char **argv)
{
  int err;
  char *filename;
  ta_bars_t *bars;
  double *williamsr;
  int period = 14;
  int i;

  if (argc < 2) {
    fprintf(stderr, "usage: calcwilliamsr <filename>\n");
    putchar('\n');
    fputs("Example:\n", stdout);
    putchar('\n');
    fputs("   calcwilliamsr BANCOPAN.csv\n", stdout);
    return 1;
  }

  filename = argv[1];

  /* Read bar data from CSV file. */
  err = read_csv(filename, &bars);
  if (err < 0) {
    return err;
  }

  /* Allocate memory for William's %R array. */
  williamsr = malloc(sizeof(double) * bars->numrows);
  if (williamsr == NULL) {
    fprintf(stderr, "Out of memory\n");
    return 1;
  }

  /* Calculate William's %R. */
  ind_williamsr(bars->high, bars->low, bars->close, bars->numrows, period, williamsr);

  /* Output values. */
  for (i = period; i < bars->numrows; i++) {
    printf("%f\n", *(williamsr + i));
  }

  /* Free memory allocated for William's %R array. */
  free(williamsr);

  return 0;
}
