#include "csv_helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stock.h"

void process_field(void *field,
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
 
  switch (stock->cur_field) {
  case DATE:
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
 
  case OPEN:
  case HIGH:
  case LOW:
  case CLOSE:
  case ADJ_CLOSE:
    if (strcmp((char*)field, "null") != 0) {
      dval = strtod((char*)field, &endptr);
      if (*endptr != '\0') {
        fprintf(stderr,
                "non-float value in record %zu, field %u: \"%s\"\n",
                 stock->ticks_length + 1, stock->cur_field + 1, field);
        stock->error = 1;
        return;
      }
 
      if (stock->cur_field == OPEN)
        cur_tick->open = dval;
      else if (stock->cur_field == HIGH)
        cur_tick->high = dval;
      else if (stock->cur_field == LOW)
        cur_tick->low = dval;
      else if (stock->cur_field == CLOSE)
        cur_tick->close = dval;
      else if (stock->cur_field == ADJ_CLOSE)
        cur_tick->adj_close = dval;
    }
  case VOLUME:
    cur_tick->volume = atoi((char*)field);
  }
 

  if (stock->cur_field == VOLUME) stock->ticks_length++;
  stock->cur_field = (stock->cur_field + 1) % 7;
}
 
void process_row(int delim UNUSED, void *ctx) {
  stock_info_t *stock = (stock_info_t *)ctx;
  if (stock->error) return;
 
  if (stock->cur_field != DATE) {
    fprintf(stderr, "not enough fields in row %zu\n", stock->ticks_length);
    stock->error = 1;
  }
}
