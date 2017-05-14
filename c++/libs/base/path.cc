#include "path.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

std::string MakeAbsolute(const char *path) {
  char full_path[PATH_MAX];
  if (realpath(path, full_path) == NULL) {
    fprintf(stderr, "realpath('%s') failed\n", path);
    return std::string();
  }
  return std::string(full_path);
}

bool PathExists(const std::string& path) {
  return access(path.c_str(), F_OK) == 0;
}

bool GetRealPath(const std::string& path, std::string *out) {
  char buf[PATH_MAX];
  if (!realpath(path.c_str(), buf)) {
    return false;
  }
  *out = buf;
  return true;
}
