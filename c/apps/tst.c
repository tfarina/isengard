/*https://www.quora.com/How-do-I-read-a-csv-file-and-then-use-those-fields-to-create-an-array-structure-in-C*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "csv.h"
 
static char *read_file(char *filename, size_t *len) {
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
  char *city;
  char *country;
  double lat;
  double lon;
} city_data;
 
typedef enum {
  CITY, COUNTRY, LAT, LON
} field;
 
typedef struct {
  city_data *cities;
  size_t cities_alloc;
  size_t cities_used;
  field cur_field;
  int error;
} city_data_reader;
 
void process_field(void *field,
		   size_t field_len __attribute__((unused)),
		   void *ctx)
{
  city_data_reader *cdr = (city_data_reader*)ctx;
  if (cdr->error) return;
 
  city_data *cur_city = cdr->cities + cdr->cities_used;
 
  // used for parsing floating-point values:
  // (declaring these in a switch/case is annoying)
  char *endptr;
  double dval;
 
  switch (cdr->cur_field) {
  case CITY:
    // start of a new record; check if we need to reallocate
    if (cdr->cities_used == cdr->cities_alloc) {
      cdr->cities_alloc *= 2;
      cdr->cities = realloc(cdr->cities,
			    sizeof(city_data) * cdr->cities_alloc);
      if (cdr->cities == NULL) {
	fprintf(stderr,
		"failed to reallocate %zu bytes for city data: ",
		sizeof(city_data) * cdr->cities_alloc);
	perror(NULL);
	cdr->error = 1;
	return;
      }
      cur_city = cdr->cities + cdr->cities_used;
    }
 
    // anyway, we just got city data
    cur_city->city = strdup((char*)field);
    break;
 
  case COUNTRY:
    // country data
    cur_city->country = strdup((char*)field);
    break;
 
  case LAT:
  case LON:
    // latitude or longitude
    dval = strtod((char*)field, &endptr);
    if (*endptr != '\0') {
      fprintf(stderr,
	      "non-float value in record %zu, field %u: \"%s\"\n",
	      cdr->cities_used+1, cdr->cur_field+1, field);
      cdr->error = 1;
      return;
    }
 
    if (cdr->cur_field == LAT)
      cur_city->lat = dval;
    else
      cur_city->lon = dval;
  }
 
  if (cdr->cur_field == LON) cdr->cities_used++;
  cdr->cur_field = (cdr->cur_field + 1) % 4;
}
 
void process_row(int delim __attribute__((unused)),
        void *ctx
		 ) {
  city_data_reader *cdr = (city_data_reader*)ctx;
  if (cdr->error) return;
 
  if (cdr->cur_field != CITY) {
    fprintf(stderr, "not enough fields in row %zu\n", cdr->cities_used);
    cdr->error = 1;
  }
}
 
int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("missing filename\n", stderr);
    return 1;
  }
 
  size_t len;
  char *csvdata = read_file(argv[1], &len);
  if (csvdata == NULL)
    return 1;
 
  struct csv_parser p;
  int rc = csv_init(&p, CSV_APPEND_NULL);
  if (rc != 0) {
    free(csvdata);
    fputs("failed to initialize CSV parser\n", stderr);
    return 1;
  }
 
  city_data_reader cdr;
  memset((void*)&cdr, 0, sizeof(city_data_reader));
  cdr.cities_alloc = 2;
  cdr.cities = malloc(cdr.cities_alloc * sizeof(city_data));
  if (cdr.cities == NULL) {
    fprintf(stderr, "failed to allocate %zu bytes for city data\n",
	    cdr.cities_alloc * sizeof(city_data));
    free(csvdata);
    return 1;
  }
 
    size_t bytes_processed =
      csv_parse(&p, (void*)csvdata, len,
                process_field, process_row, &cdr);
    rc = csv_fini(&p, process_field, process_row, &cdr);
    free(csvdata);
 
    if (cdr.error || rc != 0 || bytes_processed < len) {
      fprintf(stderr,
	      "read %zu bytes out of %zu: %s\n",
	      bytes_processed, len, csv_strerror(csv_error(&p)));
      return 1;
    }
 
    printf("cities:\n");
    size_t i;
    for (i = 0; i < cdr.cities_used; i++) {
      city_data *city = cdr.cities + i;
      printf("city=\"%s\"; country=\"%s\"; latitude=%.3lf; longitude=%.3lf\n",
	     city->city, city->country, city->lat, city->lon);
      free(city->city);
      free(city->country);
    }
    free(cdr.cities);
 
    return 0;
}
