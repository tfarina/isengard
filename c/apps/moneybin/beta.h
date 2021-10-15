#ifndef _BETA_H_INCLUDED_
#define _BETA_H_INCLUDED_ 1

#include <stddef.h>

double beta_ratio(double const *stock_ret, size_t stock_ret_size,
		  double const *index_ret, size_t index_ret_size);

#endif  /* !defined(_BETA_H_INCLUDED_) */
