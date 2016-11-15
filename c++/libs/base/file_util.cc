#include "file_util.h"

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

namespace {

bool ReadFileDescriptior(int fd, std::string* content) {
  content->clear();
  char buf[4096];

  while (int r = read(fd, buf, sizeof(buf))) {
    if (r == -1) {
      if (errno == EINTR || errno == EAGAIN) {
        continue;
      }
      return false;
    }
    content->append(buf, r);
  }
  close(fd);
  return true;
}

}  // namespace

bool ReadFile(const std::string& filename, std::string* content) {
  int fd = open(filename.c_str(), O_RDONLY);
  if (fd == -1) {
    return false;
  }
  return ReadFileDescriptior(fd, content);
}
