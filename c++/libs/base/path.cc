#include "base/path.h"

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

bool IsAbsolute(const std::string& path) {
  return !path.empty() && path[0] == '/';
}

std::string GetDirectoryName(const std::string& path) {
  size_t separator = path.rfind('/');
  if (separator == 0u) {
    return "/";
  }
  if (separator == std::string::npos) {
    return std::string();
  }
  return path.substr(0, separator);
}

std::string JoinPath(const std::string& path1, const std::string& path2) {
  if (path1.empty()) {
    // "" + "/bar"
    return path2;
  }

  if (path1[path1.size() - 1] == '/') {
    if (path2.find('/') == 0) {
      // foo/ + /bar
      return path1 + path2.substr(1);
    } else {
      // foo/ + bar
      return path1 + path2;
    }
  } else {
    if (path2.find('/') == 0) {
      // foo + /bar
      return path1 + path2;
    } else {
      // foo + bar
      return path1 + "/" + path2;
    }
  }
}
