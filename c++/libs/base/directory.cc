#include "base/directory.h"

#include <sys/stat.h>

bool IsDirectory(const std::string& path) {
  struct stat buf;
  return stat(path.c_str(), &buf) == 0 && S_ISDIR(buf.st_mode);
}
