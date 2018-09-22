#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024

#define STREAM_EOK 0
#define STREAM_EIO 1

/* From msmtp:stream.c:stream_gets(). */
int stream_read_line(FILE* file, char *data, size_t size, size_t *outlen)
{
  char c;
  size_t i;  

  i = 0;
  while (i + 1 < size) {
    if (fread(&c, sizeof(char), 1, file) == 1) {
      data[i++] = c;
      if (c == '\n') {
        break;
      }
    } else {
      if (ferror(file)) {
	return STREAM_EIO;
      } else { /* EOF */
	break;
      }
    }
  }

  data[i] = '\0';
  *outlen = i;

  return STREAM_EOK;
}

/*
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
