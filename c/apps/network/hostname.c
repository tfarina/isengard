#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  char buf[HOST_NAME_MAX];

  if (gethostname(buf, sizeof(buf)) == -1) {
    fprintf(stderr, "gethostname() failed.\n");
    exit(EXIT_FAILURE);
  }

  printf("%s\n", buf);

  return 0;
}
