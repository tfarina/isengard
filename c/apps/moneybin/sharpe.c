#include "sharpe.h"

void daily_returns(double const *data, size_t size, double *res)
{
  size_t i;

  /* The returns start at the second day. */
  for (i = 1; i < size; i++) {
    res[i - 1] = (data[i] - data[i - 1]) / data[i - 1];
  }
}

double sharpe_ratio(double avg, double stdev)
{
  double risk_free_return = 0.00038;
  double result;

  result = (avg - risk_free_return) / stdev;

  return result;
}
