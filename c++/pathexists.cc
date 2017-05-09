#include <stdio.h>
#include <unistd.h>
#include <string>

static bool PathExists(const std::string& path) {
  return access(path.c_str(), F_OK) == 0;
}

int main(int argc, char **argv) {
  if (!PathExists("non-existant-file.txt")) {
    printf("file does not exist.\n");
  } else {
    printf("file exist.\n");
  }

  return 0;
}
