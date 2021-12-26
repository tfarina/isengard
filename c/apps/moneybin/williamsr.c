#include "ta.h"

#include <stdio.h>

#include "stats.h"

/**
 * William's %R Oscillator
 *
 * Equation:
 *
 *      (H - C)
 * %R = ------- x 100
 *      (H - L)
 *
 * Where:
 *
 *  - C is the current closing price
 *  - H is the highest high over the lookback period
 *  - L is the lowest low over the lookback period
 *
 * @param[in]  arr
 * @param[in]  size
 * @param[in]  period
 * @param[out] output
 * @return 0 on success, negative number (< 0) on error.
 */
int ind_williamsr(double const *high, double const *low, double const *close, size_t size, int period, double *output)
{
  double minlow;
  double maxhigh;
  double diff1;
  double diff2;
  double fastk;
  int pos;

  for (pos = period; pos < size; pos++) {
    stats_range_min(low, size, pos - period, pos /* stop index */, &minlow);
#if 1
    printf("%.2f\n", minlow);
#endif

    stats_range_max(high, size, pos - period, pos /* stop index */, &maxhigh);
#if 2
    printf("%.2f\n", maxhigh);

    printf("%.2f\n", close[pos - 1]);
#endif
    
    diff1 = maxhigh - close[pos - 1];
    diff2 = maxhigh - minlow;

    fastk = (diff1 / diff2) * 100;
#if 2
    printf("%.2f\n", fastk);
#endif

    output[pos] = fastk;
  }

  return 0;
}
