#ifndef _SHARPE_H_INCLUDED_
#define _SHARPE_H_INCLUDED_ 1

#include <stddef.h>

void daily_returns(double const *data, size_t size, double *res);

double sharpe_ratio(double avg, double stdev);

#endif  /* !defined(_SHARPE_H_INCLUDED_) */
