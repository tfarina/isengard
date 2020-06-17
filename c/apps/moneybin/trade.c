#include "crossover.h"
#include "ta.h"

int main(void)
{
  int err;
  char filename[13] = "PETR4.SA.csv";
  ta_bars_t *bars;

  /* Read bar data from CSV file. */
  err = read_csv(filename, &bars);
  if (err < 0) {
    return err;
  }

  return 0;
}
