#include <stdio.h>

#include "os.h"

int main(int argc, char **argv) {
  char *username;

  username = os_username();

  printf("%s\n", username);

  return 0;
}
