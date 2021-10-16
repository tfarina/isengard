#include "sharpe.h"

double sharpe_ratio(double avg, double stdev, double risk_free_rate)
{
  double result;

  result = (avg - risk_free_rate) / stdev;

  return result;
}
