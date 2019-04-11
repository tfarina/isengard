#ifndef STOCK_H_
#define STOCK_H_

#include <stddef.h>

typedef struct {
  char *date;
  double open;
  double high;
  double low;
  double close;
  double adj_close;
  int volume;
} stock_tick_t;
 
typedef struct {
  char *symbol;
  stock_tick_t *ticks;
  size_t ticks_capacity;
  size_t ticks_length;
  int error;
} stock_info_t;

#endif  /* STOCK_H_ */
