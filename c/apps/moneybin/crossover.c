#include "crossover.h"

int ta_crossover(double *a1, double *a2, int pos)
{
  if (pos == 0) {
    return TA_ZERO;
  }

  if ((a1[pos] > a2[pos]) && (a1[pos - 1] < a2[pos - 1])) {
    return TA_UP;
  } else if ((a1[pos] < a2[pos]) && (a1[pos - 1] > a2[pos - 1])) {
    return TA_DOWN;
  }

  return TA_ZERO;  
}
