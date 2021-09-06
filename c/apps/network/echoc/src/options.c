#include "options.h"

/* config.h must be included first.*/
#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "attributes.h"
#include "echoc.h"
#include "getopt.h"

/* For now, no gettext support. */
#define _(STRING) STRING

static char const short_options[] =
    "h"  /* help */
    "V"  /* version */
    ;

static struct option const long_options[] = {
  { "help",        no_argument,       (int *) 0, 'h' },
  { "version",     no_argument,       (int *) 0, 'V' },
  { (char *) 0,    no_argument,       (int *) 0,  0  }
};

static MK_NORETURN void usage(int status) {
  if (status) {
    fprintf(stderr, _("Try '%s --help' for more information.\n"), progname);
  } else {
    /*
     * This is written in this way to make the life of translators easier,
     * especially those trying to translate right-to-left languages like
     * Hebrew.
     */
    printf(_("Usage: %s [OPTIONS] host [port]\n"), progname);
    putchar('\n');

    fputs(_("\
Options:\n\
"), stdout);
    fputs(_("\
  -h, --help              display this help and exit\n\
"), stdout);
    fputs(_("\
  -V, --version           output version information and exit\n\
"), stdout);
  }

  exit(status);
}

static void version(void) {
  printf("%s %s\n", progname, VERSION);
}

void parse_args(int argc, char **argv) {
  int optchr;

  for (;;) {
    optchr = getopt_long(argc, argv, short_options, long_options, NULL);
    if (optchr == -1) {
      /* no more options */
      break;
    }

    switch (optchr) {
    case 'h':  /* --help */
      usage(EXIT_SUCCESS);

    case 'V':  /* --version */
      version();
      exit(EXIT_SUCCESS);

    default:
      usage(EXIT_FAILURE);
    }
  }

  /* Takes out the program name from the argument array. */
  argc -= optind;
  argv += optind;

  /* It should have at least the destination address or hostname of the server. */
  if (argc < 1) {
    usage(0);
  }
}
