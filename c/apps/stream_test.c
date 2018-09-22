#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stream.h"

#define BUFSIZE 1024

/**
 * To test this, run:
 *
 * $ cat ~/email.txt | ./build/stream
 *
 */
int main(void)
{
  char buffer[BUFSIZE];
  size_t buflen;

  for (;;) {
    stream_read_line(stdin, buffer, sizeof(buffer), &buflen);

    if (buflen == 0) {
      break;
    }
  
    printf("%s", buffer);
  }

  return 0;
}
