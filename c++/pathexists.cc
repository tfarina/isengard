#include <stdio.h>
#include <unistd.h>
#include <string>

bool PathExists(const std::string& path) {
  return access(path.c_str(), R_OK) == 0;
}
int main(void) {
  if (!PathExists("non-existant-file.txt")) {
    printf("file does not exist.\n");
  } else {
    printf("file exist.\n");
  }

  return 0;
}
