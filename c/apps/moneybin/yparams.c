#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "getopt.h"

static char const short_options[] =
  "s:" /* symbol */
  ;

/*
 * For long options that have no equivalent short option, use a
 * non-character as a pseudo short option, starting with CHAR_MAX + 1.
*/
enum {
  START_OPTION = CHAR_MAX + 1,
  END_OPTION
};

static struct option const long_options[] = {
/*{  name,          has_arg,           *flag,     val } */
  { "symbol",      required_argument,       (int *) 0, 's' },
  { "start",       required_argument, (int *) 0, START_OPTION },
  { "end",         required_argument, (int *) 0, END_OPTION },
  { (char *) 0,    no_argument,       (int *) 0,  0  }
};

int
main(int argc, char **argv)
{
  char *symbol = NULL;
  char start_date[11];
  char end_date[11];
  int optchr;

  *start_date = 0;
  *end_date = 0;

  for (;;) {
    optchr = getopt_long(argc, argv, short_options, long_options, (int *) 0);
    /*
     * No more options. All command-line options have been parsed.
     */
    if (optchr == -1) {
      break;
    }

    switch (optchr) {
    case 's':  /* --symbol */
      symbol = optarg;
      break;

    case START_OPTION:
      strncpy(start_date, optarg, 10);
      start_date[10] = 0;
      break;
  
    case END_OPTION:
      strncpy(end_date, optarg, 10);
      end_date[10] = 0;
      break;

    default:
      break;
    } /* end switch (optchr) */
  } /* end for (;;) */

  if (symbol != NULL) {
    printf("%s\n", symbol);
  }

  if (start_date != NULL && *start_date != '\0') {
    printf("%s\n", start_date);
  }

  if (end_date != NULL && *end_date != '\0') {
    printf("%s\n", end_date);
  }

  return 0;
}
