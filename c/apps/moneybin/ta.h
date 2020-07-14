#ifndef TA_H_
#define TA_H_

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

int read_csv(char const *filename, ta_bars_t **outbars);

#endif  /* TA_H_ */
