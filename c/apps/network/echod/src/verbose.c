#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "getopt.h"

static int want_verbose = 0;

/**
 * Short options.
 */
static char const short_options[] =
  "h"  /* help */
  "v"  /* verbose */
  ;

/**
 * Long options.
 */
static struct option const long_options[] = {
/*{  name,       has_arg,       *flag, val } */
  { "help",      no_argument,       0, 'h' },
  { "verbose",   no_argument,       0, 'v' },
  { (char *) 0,  no_argument, (int *) 0, 0 }
};

/**
 * Display the help message and exit with 'status'.
 */
static void
usage(int status)
{
  printf("Usage: %s [OPTION]... FILE\n\n", "verbose");
  fputs(
"  -v, --verbose          print more information (can be used several times)\n", stdout);
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

    case 'v':  /* --verbose */
      want_verbose = 1;
      break;

    default:
      usage(EXIT_FAILURE);
    }
  }

  printf("%d\n", want_verbose);
}

int
main(int argc, char **argv)
{
  parse_args(argc, argv);

  return 0;
}
