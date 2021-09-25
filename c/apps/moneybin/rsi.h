#ifndef _RSI_H_INCLUDED_ 
#define _RSI_H_INCLUDED_

#include <stddef.h>

/**
 * Relative Strength Index
 *
 * @param[in]  input
 * @param[in]  size
 * @param[in]  period
 * @param[out] output
 *
 * @return relative strength index
 */
int ind_rsi(double const *input, size_t size, int period, double *output);

#endif  /* !defined(_RSI_H_INCLUDED_) */
