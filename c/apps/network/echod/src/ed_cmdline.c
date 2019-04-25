#include "ed_cmdline.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "ed_rcode.h"
#include "ed_utils.h"

#define CRLF "\x0d\x0a"

int show_help = 0;
int show_version = 0;

static char short_options[] =
    "h"  /* help */
    "V"  /* version */
    "d"  /* daemon mode */
    "c:" /* configuration file */
    "L:" /* log file */
    "P:" /* pid file */
    "u:" /* user identity to run as */
    "l:" /* interface to listen on */
    "p:" /* tcp port number to listen on */
    "b:" /* tcp backlog queue limit */
    ;

static struct option long_options[] = {
    { "help",        no_argument,       NULL, 'h' }, /* help */
    { "version",     no_argument,       NULL, 'V' }, /* version */
    { "daemonize",   no_argument,       NULL, 'd' }, /* daemon mode */
    { "config",      required_argument, NULL, 'c' }, /* configuration file */
    { "logfile",     required_argument, NULL, 'L' }, /* log file */
    { "pidfile",     required_argument, NULL, 'P' }, /* pid file */
    { "user",        required_argument, NULL, 'u' }, /* user identity to run as */
    { "interface",   required_argument, NULL, 'l' }, /* interface to listen on */
    { "port",        required_argument, NULL, 'p' }, /* tcp port number to listen on */
    { "backlog",     required_argument, NULL, 'b' }, /* tcp backlog queue limit */
    { NULL,          0,                 NULL,  0  }
};

int ed_cmdline_parse(int argc, char **argv, char const *program_name, ed_config_t *config) {
  int c, value;

  opterr = 0;

  for (;;) {
    c = getopt_long(argc, argv, short_options, long_options, NULL);
    if (c == -1) {
      /* no more options */
      break;
    }

    switch (c) {
    case 'h':
      show_help = 1;
      break;

    case 'V':
      show_version = 1;
      break;

    case 'd':
      config->daemonize = 1;
      break;

    case 'c':
      config->conf_filename = optarg;
      break;

    case 'L':
      config->logfile = optarg;
      break;

    case 'P':
      config->pidfile = optarg;
      break;

    case 'u':
      config->username = optarg;
      break;

    case 'l':
      config->interface = optarg;
      break;

    case 'p':
      value = atoi(optarg);
      if (value <= 0) {
	fprintf(stderr, "%s: option -p requires a non zero number\n", program_name);
	return ED_ERROR;
      }
      if (!ed_valid_port(value)) {
	fprintf(stderr, "%s: option -s value %d is not a valid port\n", program_name, value);
	return ED_ERROR;
      }

      config->port = value;
      break;

    case 'b':
      value = atoi(optarg);
      if (value <= 0) {
	fprintf(stderr, "%s: option -b requires a non zero number\n", program_name);
	return ED_ERROR;
      }

      config->backlog = value;
      break;

    case '?':
      fprintf(stderr, "%s: invalid option -- '%c'\n", program_name, optopt);
      return ED_ERROR;

    default:
      fprintf(stderr, "%s: invalid option -- '%c'\n", program_name, optopt);
      return ED_ERROR;
      /* NOTREACHED */
    }
  }

  return ED_OK;
}

void ed_cmdline_help(char const *program_name) {
  fprintf(stderr,
	  "usage: %s [-hd] [-L log file] [-P pid file] [-u user]" CRLF
	  "             [-l interface] [-p port] [-b backlog]" CRLF CRLF,
	  program_name);
  fprintf(stderr,
	  "options:" CRLF
          "  -h, --help              show usage, options and exit" CRLF
          "  -V, --version           show version and exit" CRLF
          "  -d, --daemonize         run as a daemon" CRLF
          "  -c, --config=S          set configuration file" CRLF
          "  -L, --logfile=S         set the debug logging file (default: %s)" CRLF
          "  -P, --pidfile=S         store pid in a file (default: not stored)" CRLF
          "  -u, --user=S            user identity to run as" CRLF
          "  -l, --interface=S       interface to listen on (default: %s)" CRLF
          "  -p, --port=N            set the tcp port to listen on (default: %d)" CRLF
          "  -b, --backlog=N         the backlog argument of listen() applied to the" CRLF
          "                          listening socket (default: %d)" CRLF
	  "",
          "stderr",
          ED_DEFAULT_LISTEN_ADDR,
          ED_DEFAULT_LISTEN_PORT,
          ED_DEFAULT_LISTEN_BACKLOG
	  );
}
