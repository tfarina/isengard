#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "getopt.h"

static int want_quiet = 0;

/**
 * Short options.
 */
static char const short_options[] =
  "h"  /* help */
  "q"  /* quiet or silent */
  ;

/**
 * Long options.
 */
static struct option const long_options[] = {
/*{  name,       has_arg,       *flag, val } */
  { "help",      no_argument,       0, 'h' },
  { "quiet",     no_argument,       0, 'q' },
  { "silent",    no_argument,       0, 'q' },
  { (char *) 0,  no_argument, (int *) 0, 0 }
};

/**
 * Display the help message and exit with 'status'.
 */
static void
usage(int status)
{
  printf("Usage: %s [OPTION]... FILE\n\n", "quiet");
  fputs(
"  -q, --quiet, --silent  suppress non-error messages\n", stdout);
  fputs(
"  -h, --help             display this help and exit\n", stdout);

  exit(status);
}

static void
parse_args(int argc, char **argv)
{
  int optchr;

  optind = 0;

  for (;;) {
    optchr = getopt_long(argc, argv, short_options, long_options, (int *) 0);
    /*
     * No more options. All command-line options have been parsed.
     */
    if (optchr == -1) {
      break;
    }

    switch (optchr) {
    case 'h':
      usage(EXIT_SUCCESS);

    case 'q':  /* --quiet, --silent */
      want_quiet = 1;
      break;

    default:
      usage(EXIT_FAILURE);
    }
  }

  printf("%d\n", want_quiet);
}

int
main(int argc, char **argv)
{
  parse_args(argc, argv);

  return 0;
}
