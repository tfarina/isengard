#include "base64.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
  char *user;
  size_t userlen;
  size_t b64len;
  char *b64;

  if (argc < 2) {
    fprintf(stderr, "usage: encode <string>\n");
    return 1;
  }

  user = argv[1];
  userlen = strlen(user);
  b64len = BASE64_LENGTH(userlen);
  b64 = malloc(b64len + 1);

  base64_encode(user, strlen(user), b64, b64len);

  printf("%s\n", b64);

  return 0;
}
