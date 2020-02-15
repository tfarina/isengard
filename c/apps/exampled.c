#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syslog.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (daemon(0, 0) == -1) {
    fprintf(stderr, "%s: unable to daemonize: %s\n", argv[0], strerror(errno));
    exit(EXIT_FAILURE);
  }

  openlog("exampled", LOG_PID | LOG_NDELAY, LOG_DAEMON);
  syslog(LOG_INFO, "I'M A DAEMON PROCESS");
  closelog();

  while (1) {
    sleep(1);
  }

  return 0;
}
