/*https://www.quora.com/How-do-I-read-a-csv-file-and-then-use-those-fields-to-create-an-array-structure-in-C*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csv.h"

static char *readfile(char *filename, size_t *len) {
  // open file for reading
  FILE *fh = fopen(filename, "r");
  if (fh == NULL) {
    perror("fopen()");
    return NULL;
  }
 
  int rc;
 
  // get file length
  rc = fseek(fh, 0, SEEK_END);
  if (rc < 0) {
    perror("fseek(END)");
    return NULL;
  }
  long l = ftell(fh);
  if (l < 0) {
    perror("ftell()");
    return NULL;
  }
  *len = l;
 
  // return file pointer to the beginning of input
  rc = fseek(fh, 0, SEEK_SET);
  if (rc < 0) {
    perror("fseek(SET)");
    return NULL;
  }
 
  // read in all file contents
  char *contents = malloc(*len);
  if (contents == NULL) {
    perror("malloc");
    return NULL;
  }
  size_t read = 0;
  while (read < *len) {
    size_t r = fread(contents + read, 1, *len - read, fh);
    if (r == 0) {
      if (ferror(fh)) {
	fputs("error reading input\n", stderr);
	free(contents);
	fclose(fh);
	return NULL;
      } else if (feof(fh)) {
	fprintf(stderr,
		"EOF encountered after %zu bytes (expected %zu)\n",
		read, *len);
	*len = read;
	break;
      }
    }
    read += r;
  }
 
  fclose(fh);
  return contents;
}
 
typedef struct {
  char *date;
  double open;
  double high;
  double low;
  double close;
  double adj_close;
  int volume;
} stock_data_t;
 
typedef enum {
  DATE, OPEN, HIGH, LOW, CLOSE, ADJ_CLOSE, VOLUME
} field;
 
typedef struct {
  char *symbol;
  stock_data_t *ticks;
  size_t ticks_alloc;
  size_t ticks_used;
  field cur_field;
  int error;
} stock_info_t;
 
void process_field(void *field,
		   size_t field_len __attribute__((unused)),
		   void *ctx)
{
  stock_info_t *stock = (stock_info_t *)ctx;
  if (stock->error) return;
 
  stock_data_t *cur_tick = stock->ticks + stock->ticks_used;

  // used for parsing floating-point values:
  // (declaring these in a switch/case is annoying)
  char *endptr;
  double dval;
 
  switch (stock->cur_field) {
  case DATE:
    // start of a new record; check if we need to reallocate
    if (stock->ticks_used == stock->ticks_alloc) {
      stock->ticks_alloc *= 2;
      stock->ticks = realloc(stock->ticks,
			     sizeof(stock_data_t) * stock->ticks_alloc);
      if (stock->ticks == NULL) {
	fprintf(stderr,
		"failed to reallocate %zu bytes for stock data: ",
		sizeof(stock_data_t) * stock->ticks_alloc);
	perror(NULL);
	stock->error = 1;
	return;
      }
      cur_tick = stock->ticks + stock->ticks_used;
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
	        stock->ticks_used + 1, stock->cur_field + 1, field);
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
 

  if (stock->cur_field == VOLUME) stock->ticks_used++;
  stock->cur_field = (stock->cur_field + 1) % 7;
}
 
void process_row(int delim __attribute__((unused)), void *ctx) {
  stock_info_t *stock = (stock_info_t *)ctx;
  if (stock->error) return;
 
  if (stock->cur_field != DATE) {
    fprintf(stderr, "not enough fields in row %zu\n", stock->ticks_used);
    stock->error = 1;
  }
}

int main(int argc, char **argv) {
  char *csvdata;
  size_t len;
  int rc;
  struct csv_parser parser;
  stock_info_t stock;
  size_t bytes_processed;
  size_t i;

  if (argc != 3) {
    fputs("usage: ticks filename.csv SYMBOL\n", stderr);
    return 1;
  }

  csvdata = readfile(argv[1], &len);
  if (csvdata == NULL)
    return 1;

  rc = csv_init(&parser, CSV_APPEND_NULL);
  if (rc != 0) {
    free(csvdata);
    fputs("failed to initialize CSV parser\n", stderr);
    return 1;
  }
 
  memset((void *)&stock, 0, sizeof(stock_info_t));
  stock.symbol = strdup(argv[2]);
  stock.ticks_alloc = 2;
  stock.ticks = malloc(stock.ticks_alloc * sizeof(stock_data_t));
  if (stock.ticks == NULL) {
    fprintf(stderr, "failed to allocate %zu bytes for stock data\n",
	    stock.ticks_alloc * sizeof(stock_data_t));
    free(csvdata);
    return 1;
  }
 
  bytes_processed = csv_parse(&parser, (void*)csvdata, len,
                              process_field, process_row, &stock);
  rc = csv_fini(&parser, process_field, process_row, &stock);
  free(csvdata);
 
  if (stock.error || rc != 0 || bytes_processed < len) {
    fprintf(stderr,
            "read %zu bytes out of %zu: %s\n",
	    bytes_processed, len, csv_strerror(csv_error(&parser)));
    return 1;
  }
 
  printf("%s ticks:\n", stock.symbol);

  for (i = 0; i < stock.ticks_used; i++) {
    stock_data_t *tick = stock.ticks + i;
    printf("date=\"%s\"; open=%.4lf; high=%.4lf; low=%.4lf; close=%.4lf; adj_close=%.4lf; volume=%d\n",
           tick->date, tick->open, tick->high, tick->low, tick->close, tick->adj_close, tick->volume);
    free(tick->date);
  }
  free(stock.ticks);
 
  return 0;
}
