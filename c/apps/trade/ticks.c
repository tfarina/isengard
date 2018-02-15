/*https://www.quora.com/How-do-I-read-a-csv-file-and-then-use-those-fields-to-create-an-array-structure-in-C*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csv.h"

char *g_symbol;

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
  char *symbol;
  char *date;
  double open;
  double high;
  double low;
  double close;
  double adj_close;
  int volume;
} tickerdata;
 
typedef enum {
  DATE, OPEN, HIGH, LOW, CLOSE, ADJ_CLOSE, VOLUME
} field;
 
typedef struct {
  tickerdata *ticks;
  size_t ticks_alloc;
  size_t ticks_used;
  field cur_field;
  int error;
} tickerdata_reader;
 
void process_field(void *field,
		   size_t field_len __attribute__((unused)),
		   void *ctx)
{
  tickerdata_reader *cdr = (tickerdata_reader*)ctx;
  if (cdr->error) return;
 
  tickerdata *cur_tick = cdr->ticks + cdr->ticks_used;
 
  cur_tick->symbol = g_symbol;

  // used for parsing floating-point values:
  // (declaring these in a switch/case is annoying)
  char *endptr;
  double dval;
 
  switch (cdr->cur_field) {
  case DATE:
    // start of a new record; check if we need to reallocate
    if (cdr->ticks_used == cdr->ticks_alloc) {
      cdr->ticks_alloc *= 2;
      cdr->ticks = realloc(cdr->ticks,
			    sizeof(tickerdata) * cdr->ticks_alloc);
      if (cdr->ticks == NULL) {
	fprintf(stderr,
		"failed to reallocate %zu bytes for city data: ",
		sizeof(tickerdata) * cdr->ticks_alloc);
	perror(NULL);
	cdr->error = 1;
	return;
      }
      cur_tick = cdr->ticks + cdr->ticks_used;
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
	        cdr->ticks_used+1, cdr->cur_field+1, field);
        cdr->error = 1;
        return;
      }
 
      if (cdr->cur_field == OPEN)
        cur_tick->open = dval;
      else if (cdr->cur_field == HIGH)
        cur_tick->high = dval;
      else if (cdr->cur_field == LOW)
        cur_tick->low = dval;
      else if (cdr->cur_field == CLOSE)
        cur_tick->close = dval;
      else if (cdr->cur_field == ADJ_CLOSE)
        cur_tick->adj_close = dval;
    }
  case VOLUME:
    cur_tick->volume = atoi((char*)field);
  }
 

  if (cdr->cur_field == VOLUME) cdr->ticks_used++;
  cdr->cur_field = (cdr->cur_field + 1) % 7;
}
 
void process_row(int delim __attribute__((unused)), void *ctx) {
  tickerdata_reader *cdr = (tickerdata_reader*)ctx;
  if (cdr->error) return;
 
  if (cdr->cur_field != DATE) {
    fprintf(stderr, "not enough fields in row %zu\n", cdr->ticks_used);
    cdr->error = 1;
  }
}

int main(int argc, char **argv) {
  size_t len;
  struct csv_parser p;
  tickerdata_reader cdr;
  size_t i;

  if (argc != 3) {
    fputs("missing filename and symbol name\n", stderr);
    return 1;
  }

  g_symbol = strdup(argv[2]);
 
  char *csvdata = readfile(argv[1], &len);
  if (csvdata == NULL)
    return 1;

  int rc = csv_init(&p, CSV_APPEND_NULL);
  if (rc != 0) {
    free(csvdata);
    fputs("failed to initialize CSV parser\n", stderr);
    return 1;
  }
 
  memset((void*)&cdr, 0, sizeof(tickerdata_reader));
  cdr.ticks_alloc = 2;
  cdr.ticks = malloc(cdr.ticks_alloc * sizeof(tickerdata));
  if (cdr.ticks == NULL) {
    fprintf(stderr, "failed to allocate %zu bytes for city data\n",
	    cdr.ticks_alloc * sizeof(tickerdata));
    free(csvdata);
    return 1;
  }
 
  size_t bytes_processed = csv_parse(&p, (void*)csvdata, len,
                                     process_field, process_row, &cdr);
  rc = csv_fini(&p, process_field, process_row, &cdr);
  free(csvdata);
 
  if (cdr.error || rc != 0 || bytes_processed < len) {
    fprintf(stderr,
            "read %zu bytes out of %zu: %s\n",
	    bytes_processed, len, csv_strerror(csv_error(&p)));
    return 1;
  }
 
  printf("%s ticks:\n", g_symbol);

  for (i = 0; i < cdr.ticks_used; i++) {
    tickerdata *tick = cdr.ticks + i;
    printf("date=\"%s\"; open=%.4lf; high=%.4lf; low=%.4lf; close=%.4lf; adj_close=%.4lf; volume=%d\n",
           tick->date, tick->open, tick->high, tick->low, tick->close, tick->adj_close, tick->volume);
    free(tick->date);
  }
  free(cdr.ticks);
 
  return 0;
}
