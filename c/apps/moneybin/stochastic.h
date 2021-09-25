#ifndef _STOCHASTIC_H_INCLUDED_ 
#define _STOCHASTIC_H_INCLUDED_ 1

#include <stddef.h>

/**
 * Stochastic Oscillator
 *
 * @param[in]  arr
 * @param[in]  size
 * @param[in]  period
 * @param[out] output
 *
 * @return stochastic
 */
int ind_stochastic(double const *high, double const *low, double const *close, size_t size, int period, double *output);

#endif  /* !defined(_STOCHASTIC_H_INCLUDED_) */
