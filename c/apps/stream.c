#include "stream.h"

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
