#include <stdio.h>
#include <time.h>

#include "iso8601.h"

int main(int argc, char **argv) {
  char *start;
  time_t start_time;
  char samestart[32];
  
  if (argc < 2) {
    fputs("usage: strtotime YYYY-MM-DD\n", stderr);
    return 1;
  }

  start = argv[1];

  start_time = str_to_time(start);

  time_to_str(start_time, samestart, sizeof(samestart));

  printf("%s\n", samestart);

  return 0;
}
