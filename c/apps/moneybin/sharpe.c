#include "sharpe.h"

double sharpe_ratio(double avg, double stdev)
{
  double risk_free_return = 0.00038;
  double result;

  result = (avg - risk_free_return) / stdev;

  return result;
}
