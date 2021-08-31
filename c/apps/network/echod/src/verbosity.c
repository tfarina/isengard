#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "getopt.h"

static int log_level = 2;

/**
 * Display the help message and exit with 'status'.
 */
static void
usage(int status)
{
  printf("Usage: %s [OPTION]... FILE\n\n", "verbosity");
  fputs(
"  -v[v...], --verbosity  increase verbosity (can use multiple v)\n"
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

  static char const short_options[] = "hv::";
  static struct option const long_options[] = {
    { "help",      no_argument,       0, 'h' },
    { "verbosity", optional_argument, 0, 'v' },
    { (char *) 0, no_argument, (int *) 0, 0 }
  };

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
    default:
      usage(EXIT_FAILURE);

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
