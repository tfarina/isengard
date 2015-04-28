#include <stdio.h>

#include <string>

#include "read_file.h"

int main(int argc, char **argv) {
  std::string content;
  ReadFile(std::string(argv[1]), &content);
  printf("%s\n", content.c_str());

  return 0;
}
