#include "file_util.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
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

bool WriteFile(const std::string& path, const std::string& contents) {
  FILE* fp = fopen(path.c_str(), "w");
  if (fp == NULL) {
    fprintf(stderr, "WriteFile(%s): Unable to create file. %s",
            path.c_str(), strerror(errno));
    return false;
  }

  if (fwrite(contents.data(), 1, contents.length(), fp) < contents.length()) {
    fprintf(stderr, "WriteFile(%s): Unable to write to the file. %s",
            path.c_str(), strerror(errno));
    fclose(fp);
    return false;
  }

  if (fclose(fp) == EOF) {
    fprintf(stderr, "WriteFile(%s): Unable to close the file. %s",
            path.c_str(), strerror(errno));
    return false;
  }

  return true;
}

bool GetCurrentDirectory(std::string *dir) {
  char buf[PATH_MAX];
  if (!getcwd(buf, sizeof(buf))) {
    //NOTREACHED();
    return false;
  }
  *dir = std::string(buf);
  return true;
}
