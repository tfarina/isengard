/* https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/ */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

#include "fnet.h"
#include "os_path.h"
#include "validate.h"

#define BUFSIZE 4096

#define CRLF "\x0d\x0a"

#define DEF_ECHO_PORT 7

static char const *progname;

static char short_options[] =
    "h"   /* help */
    ;

static struct option long_options[] = {
    { "help",        no_argument,       NULL, 'h' }, /* help */
    { NULL,          0,                 NULL,  0  }
};

static void usage(char const *program_name) {
  fprintf(stderr,
	  "usage: %s [OPTION] host port..." CRLF CRLF,
	  program_name);
  fprintf(stderr,
	  "options:" CRLF
          "  -h, --help              display this help and exit" CRLF
	  "");
}

int main(int argc, char **argv) {
  int optchr, value;
  int sd;
  int err;
  char sendline[BUFSIZE];
  char recvline[BUFSIZE];
  char *host;
  int port;

  progname = os_path_basename(*(argv + 0));

  port = DEF_ECHO_PORT;

  for (;;) {
    optchr = getopt_long(argc, argv, short_options, long_options, NULL);
    if (optchr == -1) {
      /* no more options */
      break;
    }

    switch (optchr) {
    case 'h':
      usage(progname);
      return EXIT_SUCCESS;

    case '?':
      usage(progname);
      return EXIT_SUCCESS;

    default:
      fprintf(stderr, "%s: invalid option -- '%c'", progname, optopt);
      return -1;
      /* NOTREACHED */
    }
  }

  /* Takes out the program name from the argument array. */
  argc -= optind;
  argv += optind;

  if (argc != 2) {
    usage(progname);
    return -1;
  }

  host = argv[0];

  value = atoi(argv[1]);
  if (value <= 0) {
    fprintf(stderr, "%s: port requires a non zero number\n", progname);
    return EXIT_FAILURE;
  }
  if (!valid_port(value)) {
    fprintf(stderr, "%s: %d is not a valid port\n", progname, value);
    return EXIT_FAILURE;
  }

  port = value;

  err = fnet_tcp_socket_connect(host, port, &sd);
  if (err < 0) {
    return -1;
  }

  memset(sendline, 0, sizeof(sendline));
  memset(recvline, 0, sizeof(recvline));

  while (1) {
    fgets(sendline, BUFSIZE, stdin); /*stdin = 0 , for standard input */

    write(sd, sendline, strlen(sendline) + 1);

    read(sd, recvline, BUFSIZE);
    fputs(recvline, stdout);
  }

  return 0;
}
