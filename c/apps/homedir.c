#include <stdio.h>
#include <stdlib.h>

/**
 * Gets the current user's home directory.
 */
static const char *get_home_dir(void)
{
  return getenv("HOME");
}

int main(int argc, char **argv) {
  const char *homedir;

  homedir = get_home_dir();

  printf("%s\n", homedir);

  return 0;
}
