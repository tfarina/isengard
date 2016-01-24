#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

static std::string MakeAbsolute(const char *path) {
  char full_path[PATH_MAX];
  if (realpath(path, full_path) == NULL) {
    fprintf(stderr, "realpath('%s') failed\n", path);
    return std::string();
  }
  return std::string(full_path);
}

int main(int argc, char **argv) {
  std::string absolute_path = MakeAbsolute("../..");

  printf("%s\n", absolute_path.c_str());

  return 0;
}
