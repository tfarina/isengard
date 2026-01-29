#ifndef FINANCE_H
#define FINANCE_H

#include <stddef.h>

double break_even(double buy_price, double commission, int num_shares);

void daily_returns(double const *data, size_t size, double *res);

#endif /* FINANCE_H */
