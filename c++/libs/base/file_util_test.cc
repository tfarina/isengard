#include "file_util.h"

#include <stdio.h>
#include <string>

int main(int argc, char **argv) {
  std::string content;

  ReadFile(std::string(argv[1]), &content);

  printf("%s\n", content.c_str());

  WriteFile("test.txt", content);

  return 0;
}
