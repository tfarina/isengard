#include "options.h"

/* config.h must be included first. */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#else
#include "getopt.h"
#endif

#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "echod.h"
#include "pathnames.h"
#include "valid.h"

options_t opt;

/* User for running the Echo daemon. */
#define DEF_DAEMON_USER "_echod"

#define DEF_BIND_ADDR "0.0.0.0"
#define DEF_ECHO_PORT 7
#define DEF_BACKLOG   32

static char const short_options[] =
  "C:" /* configuration file */
  "L:" /* log file */
  "P:" /* pid file */
  "S"  /* show configuration */
  "V"  /* version */
  "a:" /* local address to bind */
  "b:" /* tcp backlog queue limit */
  "f"  /* foreground mode */
  "h"  /* help */
  "p:" /* tcp port number to listen on */
  "u:" /* user identity to run as */
  ;

/*
 * For long options that have no equivalent short option, use a
 * non-character as a pseudo short option, starting with CHAR_MAX + 1.
*/
enum {
  DUMP_OPTIONS_OPTION = CHAR_MAX + 1
};

static struct option const long_options[] = {
/*{  name,          has_arg,           *flag,     val } */
  { "config",       required_argument, (int *) 0, 'C' },
  { "logfile",      required_argument, (int *) 0, 'L' },
  { "pidfile",      required_argument, (int *) 0, 'P' },
  { "dump-options", no_argument,       (int *) 0, DUMP_OPTIONS_OPTION },
  { "version",      no_argument,       (int *) 0, 'V' },
  { "address",      required_argument, (int *) 0, 'a' },
  { "backlog",      required_argument, (int *) 0, 'b' },
  { "foreground",   no_argument,       (int *) 0, 'f' },
  { "help",         no_argument,       (int *) 0, 'h' },
  { "port",         required_argument, (int *) 0, 'p' },
  { "user",         required_argument, (int *) 0, 'u' },
  { (char *) 0,     no_argument,       (int *) 0,  0  }
};

static void errx(int const code, char const *format, ...) {
  va_list ap;

  va_start(ap, format);
  fprintf(stderr, "error: ");
  vfprintf(stderr, format, ap);
  fprintf(stderr, "\n");
  va_end(ap);

  exit(code);
}

static void usage(int status) {
  if (status) {
    fprintf(stderr, "Try '%s --help' for more information.\n", progname);
  } else {
    /*
     * This is written in this way to make the life of translators easier,
     * especially those trying to translate right-to-left languages like
     * Hebrew.
     */
    printf("Usage: %s [OPTIONS]...\n", progname);
    putchar('\n');

    fputs("\
Mandatory arguments to long options are mandatory for short options too.\n\
", stdout);
    putchar('\n');

    fputs("\
Options:\n\
", stdout);
    fputs("\
  -f, --foreground           run in the foreground\n\
", stdout);
    fprintf(stdout, "\
  -u, --user=<username>      run %s as this user after init\n\
", progname);
    fprintf(stdout, "\
  -C, --config=<file>        specify alternate configuration file [default: %s]\n\
", DEF_CFGFILE);
    fprintf(stdout, "\
  -P, --pidfile=<file>       write process id to the specified file [default: %s]\n\
", DEF_PIDFILE);
    fputs("\
  -L, --logfile=<file>       write log messages to the specified file\n\
", stdout);
    fputs("\
  -a, --address=<ip/domain>  bind to the specified address\n\
", stdout);
    fprintf(stdout, "\
  -p, --port=<number>        specify the port to listen on [default: %d]\n\
", DEF_ECHO_PORT);
    fputs("\
  -b, --backlog=<number>     set the backlog queue limit\n\
", stdout);
    fputs("\
  -h, --help                 display this help and exit\n\
", stdout);
    fputs("\
  -V, --version              output version information and exit\n\
", stdout);
  }

  exit(status);
}

static void version(void) {
  printf("%s %s\n", progname, PACKAGE_VERSION);
}

void init_options(void) {
  opt.detach = 1;
  opt.conffile = DEF_CFGFILE;
  opt.pidfile = DEF_PIDFILE;
  opt.user = DEF_DAEMON_USER;
  opt.address = DEF_BIND_ADDR;
  opt.port = DEF_ECHO_PORT;
  opt.backlog = DEF_BACKLOG;
  opt.logfile = (char *) 0;
}

void dump_options(void) {
  fprintf(stdout, "\n *** %s configuration ***\n", progname);
  fprintf(stdout, "ConfigFile    = %s\n", opt.conffile);
  fprintf(stdout, "PIDFile       = %s\n", opt.pidfile);
  fprintf(stdout, "LogFile       = %s\n", opt.logfile);
  fprintf(stdout, "User          = %s\n", opt.user);
  fprintf(stdout, "Port          = %d\n", opt.port);
  fprintf(stdout, "Backlog       = %d\n", opt.backlog);
}

void preparse_args(int argc, char **argv) {
  int optchr;

  /*
   * This separate getopt_long is needed to find the user config file
   * option ("--config") and parse it before the other user options.
   */
  for (;;) {
    optchr = getopt_long(argc, argv, short_options, long_options, (int *) 0);
    if (optchr == -1) {
      /* no more options */
      break;
    }

    switch (optchr) {
    case 'C': /* --config */
      opt.conffile = optarg;
      break;

    default:
      break;
    }
  }
}

void parse_args(int argc, char **argv) {
  int optchr, value;

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
    case 'f':  /* --foreground */
      opt.detach = 0;
      break;

    case 'C':  /* --config */
      /* This option is already parsed in preparse_args(). */
      break;

    case 'L':  /* --logfile */
      opt.logfile = optarg;
      break;

    case 'P':  /* --pidfile */
      opt.pidfile = optarg;
      break;

    case 'u':  /* --user */
      opt.user = optarg;
      break;

    case 'a':  /* --address */
      opt.address = optarg;
      break;

    case 'p':  /* --port */
      value = atoi(optarg);
      if (!valid_port(value)) {
        errx(1, "port number must be between 1 and 65535");
      }
      opt.port = value;
      break;

    case 'b':  /* --backlog */
      value = atoi(optarg);
      if (value <= 0) {
	errx(1, "option -b requires a non zero number");
      }

      opt.backlog = value;
      break;

    /* undocumented -- dump CLI options for testing */
    case DUMP_OPTIONS_OPTION:  /* --dump-options */
      opt.dump_options = 1;
      break;

    case 'h':  /* --help */
      usage(0);
      break;

    case 'V':  /* --version */
      version();
      exit(0);
      break;

    default:
      usage(1);
    }
  }
}
