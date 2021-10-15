#ifndef _FINANCE_H_INCLUDED_ 
#define _FINANCE_H_INCLUDED_ 1

#include <stddef.h>

double break_even(double buy_price, double commission, int num_shares);

void daily_returns(double const *data, size_t size, double *res);

#endif  /* !defined(_FINANCE_H_INCLUDED_) */
