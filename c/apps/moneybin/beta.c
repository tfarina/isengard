#include "beta.h"

#include "stats.h"

double beta_ratio(double const *stock_ret, size_t stock_ret_size,
		  double const *index_ret, size_t index_ret_size)
{
  double result;
  double covar, var;
  
  covar = stats_covar(stock_ret, stock_ret_size, index_ret, index_ret_size);
  var = stats_var(index_ret, index_ret_size);
 
  result = covar / var;

  return result;
}
