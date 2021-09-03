#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "getopt.h"

static int log_level = 2;

/**
 * Short options.
 */
static char const short_options[] =
  "h"    /* help */
  "v::"  /* verbosity */
  ;

/**
 * Long options.
 */
static struct option const long_options[] = {
/*{  name,       has_arg,       *flag, val } */
  { "help",      no_argument,       0, 'h' },
  { "verbosity", optional_argument, 0, 'v' },
  { (char *) 0,  no_argument, (int *) 0, 0 }
};

/**
 * Display the help message and exit with 'status'.
 */
static void
usage(int status)
{
  printf("Usage: %s [OPTION]... FILE\n\n", "verbosity");
  fputs(
"  -v[v...], --verbosity  be more verbose (use multiple times to increase verbosity)\n"
"  -vNUM, --verbosity=NUM set verbosity to NUM (higher level means more output):\n"
"                          -v1 - only errors and messages\n"
"                          -v2 - also warnings (default level)\n"
"                          -v3 - also information messages and timestamps\n"
"                          -v4 - lots of debug\n", stdout);
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

    case 'v':
      if (optarg) {
        if (optarg[0] == 'v') {
	  /* handle -vvvvv */
	  log_level += strlen(optarg) + 1;
	} else {
	  log_level = atoi(optarg);
	}
      } else {
	log_level++;
      }
      break;

    default:
      usage(EXIT_FAILURE);
    }
  }

  printf("%d\n", log_level);
}

int
main(int argc, char **argv)
{
  parse_args(argc, argv);

  return 0;
}
