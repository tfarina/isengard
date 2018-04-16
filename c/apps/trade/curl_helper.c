#include "curl_helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

size_t write_data_to_file(void *data, size_t size, size_t nmemb, void *stream)
{
  size_t written;

  written = fwrite(data, size, nmemb, (FILE *)stream);

  return written;
}

size_t write_data_to_memory(void *data, size_t size, size_t nmemb, void *ctx)
{
    buffer_t *buf = (buffer_t *)ctx;
    size_t realsize = size * nmemb;

    buf->data = (char *)realloc(buf->data, buf->length + realsize + 1);
    if (buf->data) {
        memcpy(&(buf->data[buf->length]), data, realsize);
        buf->length += realsize;
        buf->data[buf->length] = 0;
    }

    return realsize;
}
