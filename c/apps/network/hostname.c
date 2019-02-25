#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  char hostname[HOST_NAME_MAX];

  if (gethostname(hostname, sizeof(hostname)) < 0) {
    fprintf(stderr, "gethostname() failed: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  hostname[HOST_NAME_MAX - 1] = '\0';

  printf("%s\n", hostname);

  return EXIT_SUCCESS;
}
