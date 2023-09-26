#include <stdio.h>
#include <unistd.h>
#include <string>

#include "base/path.h"

int main(int argc, char **argv) {
  if (!PathExists("non-existant-file.txt")) {
    printf("file does not exist.\n");
  } else {
    printf("file exist.\n");
  }

  return 0;
}
