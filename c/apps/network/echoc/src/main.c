/* https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/ */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

#include "fnet.h"
#include "msg.h"
#include "os_path.h"
#include "validate.h"

#define BUFSIZE 4096

#define DEF_ECHO_PORT 7

static char const *progname;

static char short_options[] =
    "h"   /* help */
    ;

static struct option long_options[] = {
    { "help",        no_argument,       NULL, 'h' },
    { NULL,          0,                 NULL,  0  }
};

static void usage(char const *program_name) {
  fprintf(stderr, "Usage: %s [OPTIONS] host port\n\n", program_name);
  fprintf(stderr,
	  "Options:\n"
          "  -h, --help              display this help and exit\n"
	  "");
}

int main(int argc, char **argv) {
  int optchr, value, sockfd, err;
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
      /* Consider unrecognized options as fatal. */
      fatal("Try `--help' or man(1) echoc for more information, usage options and help.");

    default:
      /* An unrecognized option is considered a fatal error. */
      fatal("Unrecognized option.");
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

  err = fnet_tcp_socket_connect(host, port, &sockfd);
  if (err < 0) {
    return -1;
  }

  memset(sendline, 0, sizeof(sendline));
  memset(recvline, 0, sizeof(recvline));

  while (1) {
    fgets(sendline, BUFSIZE, stdin); /*stdin = 0 , for standard input */

    write(sockfd, sendline, strlen(sendline) + 1);

    read(sockfd, recvline, BUFSIZE);
    fputs(recvline, stdout);
  }

  return 0;
}
