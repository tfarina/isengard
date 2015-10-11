#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  char buf[HOST_NAME_MAX];

  if (gethostname(buf, sizeof(buf)) == -1) {
    fprintf(stderr, "gethostname() failed: %s.\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  printf("%s\n", buf);

  return 0;
}
