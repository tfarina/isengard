#include <stdlib.h>
#include <stdio.h>

#include "pivot.h"
#include "ta.h"

int main(int argc, char **argv)
{
  int err;
  char *filename;
  ta_bars_t *bars;
  float high, low, close;
  float pp, r1, r2, r3, s1, s2, s3;

  if (argc < 2) {
    fprintf(stderr, "usage: calcpivot <filename>\n");
    putchar('\n');
    fputs("Example:\n", stdout);
    putchar('\n');
    fputs("   calcpivot BPAN4.SA.csv\n", stdout);
    return 1;
  }

  filename = argv[1];

  /* Read bar data from CSV file. */
  err = read_csv(filename, &bars);
  if (err < 0) {
    return err;
  }

  close = bars->close[bars->numrows - 1];
  high = bars->high[bars->numrows - 1];
  low = bars->low[bars->numrows - 1];

  pp = classic_pp_pivot_point(high, low, close);
  s1 = classic_pp_support1(pp, high);
  r1 = classic_pp_resistance1(pp, low);
  r2 = classic_pp_resistance2(pp, r1, s1);
  r3 = classic_pp_resistance3(pp, high, low);
  s2 = classic_pp_support2(pp, r1, s1);
  s3 = classic_pp_support3(pp, high, low);

  printf("Close\tHigh\tLow\n");
  printf("%.2f\t%.2f\t%.2f\n\n", close, high, low);

  printf("S3: %.4f\nS2: %.4f\nS1: %.4f\nPP: %.4f\nR1: %.4f\nR2: %.4f\nR3: %.4f\n", s3, s2, s1, pp, r1, r2, r3);

  return 0;
}
