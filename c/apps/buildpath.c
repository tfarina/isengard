#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *build_filename(char *dirname, char *filename) {
  char *path;
  size_t len;

  len = strlen(dirname) + /* '/' */ 1 + strlen(filename) + /* '\0' */ 1;

  path = (char *) malloc(len);

  sprintf(path, "%s/%s", dirname, filename);

  return path;
}

int main(void) {
  char *path;

  path = build_filename("/etc", "resolv.conf");

  printf("%s\n", path);

  return 0;
}
