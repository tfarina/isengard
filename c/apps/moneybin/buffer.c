#include "buffer.h"

#include <stdlib.h>
#include <string.h>

void buffer_init(buffer_t *buf)
{
  buf->data = NULL;
  buf->length = 0;
}

void buffer_append(buffer_t *buf, char *data, size_t size)
{
  buf->data = (char *)realloc(buf->data, buf->length + size + 1);
  if (buf->data == NULL) {
    return;
  }

  memcpy(&(buf->data[buf->length]), data, size);
  buf->length += size;
  buf->data[buf->length] = 0;
}
