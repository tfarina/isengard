#include <stdlib.h>
#include <stdio.h>

#include "pivot.h"
#include "ta.h"

int main(int argc, char **argv)
{
  int err;
  char *filename;
  ta_bars_t *bars;
  int year, month, day;
  double high, low, close;
  double pp, r1, r2, r3, s1, s2, s3;

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

  if (0 == bars->numrows) {
    return 1;
  }

  close = bars->close[bars->numrows -1];
  high = bars->high[bars->numrows - 1];
  low = bars->low[bars->numrows - 1];

  ta_getdate(&bars->timestamp[bars->numrows - 1], &year, &month, &day);

  pp = pivot_classic_pp(high, low, close);
  r1 = pivot_classic_r1(pp, low);
  r2 = pivot_classic_r2(pp, high, low);
  r3 = pivot_classic_r3(pp, high, low);
  s1 = pivot_classic_s1(pp, high);
  s2 = pivot_classic_s2(pp, high, low);
  s3 = pivot_classic_s3(pp, high, low);

  printf("Close\tHigh\tLow\tDate\n");
  printf("%.2f\t%.2f\t%.2f\t%.4d-%.2d-%.2d\n\n", close, high, low, year, month, day);

  printf("   %7s\n", "Classic");
  printf("S3: %4.2f\n", s3);
  printf("S2: %4.2f\n", s2);
  printf("S1: %4.2f\n", s2);
  printf("PP: %4.2f\n", pp);
  printf("R1: %4.2f\n", r1);
  printf("R2: %4.2f\n", r2);
  printf("R3: %4.2f\n", r3);

  return 0;
}
