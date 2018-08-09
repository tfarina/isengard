#include "stock.h"

#include <stdio.h>
#include <stdlib.h>

void stock_ticks_alloc(stock_info_t *stock, int capacity)
{
  stock->ticks = malloc(capacity * sizeof(stock_tick_t));
  stock->ticks_capacity = capacity;
}

int stock_add_tick(MYSQL *conn, stock_info_t *stock, stock_tick_t *tick)
{
  char query[256];
  MYSQL_RES *res;

  /* Determine if this is an insert or update operation. */
  sprintf(query, "SELECT symbol FROM historicaldata WHERE symbol = \"%s\" and date = \"%s\"", stock->symbol, tick->date);
  if (mysql_query(conn, query)) {
    fprintf(stderr, "mysql: sql insert failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  res = mysql_store_result(conn);
  if (res == NULL) {
    fprintf(stderr, "mysql: mysql_store_result failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  /* If there is a record with this symbol and date, update it, otherwise insert new record. */
  if (mysql_num_rows(res)) {
    sprintf(query, "UPDATE historicaldata SET open=%f, high=%f, low=%f, close=%f, adjClose=%f, volume=%d WHERE symbol = \"%s\" and date = \"%s\"", tick->open, tick->high, tick->low, tick->close, tick->adj_close, tick->volume, stock->symbol, tick->date);
  } else {
    sprintf(query, "INSERT INTO historicaldata (symbol, date, open, high, low, close, adjClose, volume) VALUES ('%s', '%s', %f, %f, %f, %f, %f, %d)",
            stock->symbol, tick->date, tick->open, tick->high, tick->low, tick->close, tick->adj_close, tick->volume);
  }

  if (mysql_query(conn, query)) {
    fprintf(stderr, "mysql: sql operation failed: %s\n", mysql_error(conn));
    mysql_close(conn);
    return -1;
  }

  printf("%s\n", query);

  return 0;
}
