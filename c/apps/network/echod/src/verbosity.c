#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "getopt.h"

static int log_level = 2;

int
main(int argc, char **argv)
{
  int optchr;

  static char const short_options[] = "v::";
  static struct option const long_options[] = {
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

  return 0;
}
