#include <stdio.h>

#include "fstrutils.h"
#include "stats.h"
#include "ta.h"

int main(int argc, char **argv) {
  int err;
  char *filename;
  ta_bars_t *bars;
  double min;
  double max;
  double avg;
  double stdev;

  if (argc != 2) {
    fputs("usage: csvcat filename.csv\n", stderr);
    return 1;
  }

  filename = f_strdup(argv[1]);

  err = read_csv(filename, &bars);
  if (err < 0) {
    return err;
  }

  stats_min(bars->close, bars->numrows, &min);
  stats_max(bars->close, bars->numrows, &max);
  stats_avg(bars->close, bars->numrows, &avg);
  stats_stdev(bars->close, bars->numrows, &stdev);

  printf("%15s\n", "Close");
  printf("min   %9.3f\n", min);
  printf("max   %9.3f\n", max);
  printf("mean  %9.3f\n", avg);
  printf("stdev %9.3f\n", stdev);

  return 0;
}
