#ifndef _RSI_H_INCLUDED_ 
#define _RSI_H_INCLUDED_

#include <stddef.h>

/**
 * Relative Strength Index
 *
 * @param[in]  close
 * @param[in]  size
 * @param[in]  period
 * @param[out] rsi
 *
 * @return exponential moving average
 */
void ind_rsi(double const *close, size_t size, int period, double *rsi);

#endif  /* !defined(_RSI_H_INCLUDED_) */
