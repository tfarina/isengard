#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NET_MAXHOSTNAMELEN 256

int main(void) {
  char hostname[NET_MAXHOSTNAMELEN + 1];

  if (gethostname(hostname, sizeof(hostname)) < 0) {
    fprintf(stderr, "gethostname() failed: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  hostname[NET_MAXHOSTNAMELEN] = '\0'; /* Null terminate, just to be safe. */

  printf("%s\n", hostname);

  return EXIT_SUCCESS;
}
