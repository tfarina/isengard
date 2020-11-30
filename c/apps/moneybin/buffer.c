#include "buffer.h"

#include <stdlib.h>
#include <string.h>

void buffer_init(buffer_t *buf)
{
  buf->data = NULL;
  buf->size = 0;
}

void buffer_append(buffer_t *buf, char *data, size_t size)
{
  if (0 == buf->data) {
    buf->data = malloc(size + 1);
  } else {
    buf->data = (char *)realloc(buf->data, buf->size + size + 1);
    if (buf->data == NULL) {
      return;
    }
  }

  memcpy(&(buf->data[buf->size]), data, size);
  buf->size += size;
  buf->data[buf->size] = 0;
}
