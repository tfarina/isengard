#ifndef TA_H_
#define TA_H_

#include <stddef.h>

#include "timestamp.h"

/**
 * Error codes.
 */
enum {
  TA_SUCCESS = 0,    /* No error */
  TA_FAILURE = -1    /* Generic error */
};

typedef struct ta_bars_s {
  long unsigned numrows;

  timestamp_t *timestamp;
  double *open;
  double *high;
  double *low;
  double *close;
  double *adjclose;
  int *volume;
} ta_bars_t;

typedef enum ta_position_e {
  TA_POSITION_LONG,
  TA_POSITION_SHORT
} ta_position_t;

typedef enum ta_order_type_e {
  TA_ORDER_TYPE_LIMIT,
  TA_ORDER_TYPE_MARKET,
  TA_ORDER_TYPE_CANCEL
} ta_order_type_t;

typedef enum ta_order_side_e {
  TA_ORDER_SIDE_BUY,
  TA_ORDER_SIDE_SELL
} ta_order_side_t;

typedef struct ta_order_s {
  ta_order_type_t type;
  ta_order_side_t side;

  long unsigned quantity;
  double price;
} ta_order_t;

typedef enum ta_signal_e {
  TA_SIGNAL_BUY  = 0,
  TA_SIGNAL_SELL = 1,
  TA_SIGNAL_HOLD = 2
} ta_signal_t;

ta_bars_t *ta_bars_create(size_t numrows);
void ta_bars_destroy(ta_bars_t *bars);

int read_csv(char const *filename, ta_bars_t **outbars);

/* ------------------------------------------------------------------------- */
/* Indicators                                                                */
/* ------------------------------------------------------------------------- */
int ind_sma(double const *input, size_t size, int period, double *output);
int ind_ema(double const *input, size_t size, int period, double *output);
int ind_rsi(double const *input, size_t size, int period, double *output);
int ind_stochastic(double const *high, double const *low, double const *close, size_t size, int period, double *output);
int ind_williamsr(double const *high, double const *low, double const *close, size_t size, int period, double *output);

int ta_strat_rsi(double rsi, int oversold, int overbought);

#endif  /* TA_H_ */
