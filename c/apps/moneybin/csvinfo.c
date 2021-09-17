#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "third_party/libcsv/csv.h"

#include "fstrutils.h"

/* The number of rows encountered for the current file. */
static long unsigned rows;

static void
csv_row_count_cb(int c, void *data)
{
  rows++;
}

static int
read_csv(char const *filename)
{
  FILE* fp;
  struct csv_parser parser;
  char buf[1024];
  size_t bytes_read;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open file %s: %s\n", filename, strerror(errno));
    return -1;
  }

 if (csv_init(&parser, CSV_STRICT | CSV_REPALL_NL | CSV_APPEND_NULL | CSV_STRICT_FINI) != 0) {
    fprintf(stderr, "Failed to initialize csv parser\n");
    return -1;
  }

  /*
   * First we are going to read the whole csv file to gather the total
   * number of rows and fields.
   */
  while ((bytes_read = fread(buf, sizeof(char), sizeof(buf), fp)) > 0) {
    if (csv_parse(&parser, buf, bytes_read, NULL, csv_row_count_cb, NULL) != bytes_read) {
      fprintf(stderr, "Error while parsing %s: %s\n", filename, csv_strerror(csv_error(&parser)));
      csv_free(&parser);
      fclose(fp);
      return -1;
    }
  }

  if (csv_fini(&parser, NULL, csv_row_count_cb, NULL) != 0) {
    fprintf(stderr, "Error finishing CSV processing.\n");
    return -1;
  }

  csv_free(&parser);

  if (ferror(fp)) {
    fprintf(stderr, "Error reading file %s\n", filename);
    fclose(fp);
    return -1;
  }

  fclose(fp);

  return 0;
}

int
main(int argc, char **argv)
{
  char *filename;
  int retval;

  if (argc != 2) {
    fputs("usage: csvinfo filename.csv\n", stderr);
    return 1;
  }

  filename = f_strdup(argv[1]);

  retval = read_csv(filename);
  if (retval < 0) {
    return 1;
  }

  printf("%8lu", rows);
  putc('\n', stdout);

  free(filename);

  return 0;
}
