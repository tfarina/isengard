#ifndef STOCK_H_
#define STOCK_H_

#include <stddef.h>

#include <mysql/mysql.h>

typedef struct {
  char *date;
  double open;
  double high;
  double low;
  double close;
  double adj_close;
  int volume;
} stock_tick_t;
 
typedef enum {
  DATE, OPEN, HIGH, LOW, CLOSE, ADJ_CLOSE, VOLUME
} stock_tick_field_t;
 
typedef struct {
  char *symbol;
  stock_tick_t *ticks;
  size_t ticks_capacity;
  size_t ticks_length;
  stock_tick_field_t cur_field;
  int error;
} stock_info_t;

void stock_ticks_alloc(stock_info_t *stock, int capacity);
int stock_add_tick(MYSQL *conn, stock_info_t *stock, stock_tick_t *tick);
 
#endif  /* STOCK_H_ */
