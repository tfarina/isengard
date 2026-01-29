#ifndef BETA_H
#define BETA_H

#include <stddef.h>

double beta_ratio(double const *stock_ret, size_t stock_ret_size,
		  double const *index_ret, size_t index_ret_size);

#endif /* BETA_H */
