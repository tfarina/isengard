#include "csv_helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stock.h"

void csv_column_cb(void *field,
		   size_t field_len UNUSED,
		   void *ctx)
{
  stock_info_t *stock = (stock_info_t *)ctx;
  if (stock->error) return;

  stock_tick_t *cur_tick = stock->ticks + stock->ticks_length;
 
  // used for parsing floating-point values:
  // (declaring these in a switch/case is annoying)
  char *endptr;
  double dval;

  // Skip the first (header) line?
  if (strcmp((char*)field, "Date") == 0 ||
      strcmp((char*)field, "Open") == 0 ||
      strcmp((char*)field, "High") == 0 ||
      strcmp((char*)field, "Low") == 0 ||
      strcmp((char*)field, "Close") == 0 ||
      strcmp((char*)field, "Adj Close") == 0 ||
      strcmp((char*)field, "Volume") == 0) {
    return;
  }
 
  switch (stock->curr_column) {
  case CSV_COLUMN_DATE:
    /* Start of a new record; check if we need to reallocate. */
    /* If the number of entries in the vector has reached the capacity, resize. */
    if (stock->ticks_length == stock->ticks_capacity) {
      stock->ticks_capacity *= 2;
      stock->ticks = realloc(stock->ticks,
                             stock->ticks_capacity * sizeof(stock_tick_t));
      if (stock->ticks == NULL) {
        fprintf(stderr,
                "failed to reallocate %zu bytes for stock data: ",
		sizeof(stock_tick_t) * stock->ticks_capacity);
        perror(NULL);
        stock->error = 1;
	return;
      }
      cur_tick = stock->ticks + stock->ticks_length;
    }
 
    // anyway, we just got tick data
    cur_tick->date = strdup((char*)field);
    break;
 
  case CSV_COLUMN_OPEN:
  case CSV_COLUMN_HIGH:
  case CSV_COLUMN_LOW:
  case CSV_COLUMN_CLOSE:
  case CSV_COLUMN_ADJ_CLOSE:
    if (strcmp((char*)field, "null") != 0) {
      dval = strtod((char*)field, &endptr);
      if (*endptr != '\0') {
        fprintf(stderr,
                "non-float value in record %zu, field %u: \"%s\"\n",
                 stock->ticks_length + 1, stock->curr_column + 1, field);
        stock->error = 1;
        return;
      }
 
      if (stock->curr_column == CSV_COLUMN_OPEN)
        cur_tick->open = dval;
      else if (stock->curr_column == CSV_COLUMN_HIGH)
        cur_tick->high = dval;
      else if (stock->curr_column == CSV_COLUMN_LOW)
        cur_tick->low = dval;
      else if (stock->curr_column == CSV_COLUMN_CLOSE)
        cur_tick->close = dval;
      else if (stock->curr_column == CSV_COLUMN_ADJ_CLOSE)
        cur_tick->adj_close = dval;
    }
  case CSV_COLUMN_VOLUME:
    cur_tick->volume = atoi((char*)field);
  }
 

  if (stock->curr_column == CSV_COLUMN_VOLUME) {
    stock->ticks_length++;
  }

  stock->curr_column = (stock->curr_column + 1) % 7;
}
 
void csv_row_cb(int delim UNUSED, void *ctx) {
  stock_info_t *stock = (stock_info_t *)ctx;
  if (stock->error) return;
 
  if (stock->curr_column != CSV_COLUMN_DATE) {
    fprintf(stderr, "not enough fields in row %zu\n", stock->ticks_length);
    stock->error = 1;
  }
}
