#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "base/path.h"

int main(int argc, char **argv) {
  std::string absolute_path = MakeAbsolute("../..");

  printf("%s\n", absolute_path.c_str());

  return 0;
}
