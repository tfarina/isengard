#include <stdio.h>
#include <stdlib.h>

static char *get_home_dir(void) {
  return getenv("HOME");
}

int main(int argc, char **argv) {
  char *homedir = get_home_dir();

  printf("%s\n", homedir);

  return 0;
}
