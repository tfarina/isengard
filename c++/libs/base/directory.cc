#include "directory.h"

#include <sys/stat.h>

#include <vector>

#include "path.h"

bool IsDirectory(const std::string& path) {
  struct stat buf;
  return stat(path.c_str(), &buf) == 0 && S_ISDIR(buf.st_mode);
}

bool CreateDirectory(const std::string& full_path) {
  std::vector<std::string> subpaths;

  // Collect a list of all parent directories.
  std::string last_path = full_path;
  subpaths.push_back(full_path);

  for (std::string path = GetDirectoryName(full_path);
       path != last_path;
       path = GetDirectoryName(path)) {
    subpaths.push_back(path);
    last_path = path;
  }

  // Iterate through the parents and create the missing ones.
  for (std::vector<std::string>::reverse_iterator i = subpaths.rbegin(); i != subpaths.rend(); i++) {
    if (IsDirectory(*i)) {
      continue;
    }

    if (mkdir(i->c_str(), 0700) == 0) {
      continue;
    }

    // mkdir failed, but it might be due to the directory
    // appearing out of thin air. This can occur if two processes
    // are trying to create the same file system tree at the same
    // time. Check to see if it exists and make sure it is a
    // directory.
    if (!IsDirectory(*i)) {
      return false;
    }
  }

  return true;
}
