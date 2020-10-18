#include "curl_write_callbacks.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

size_t curl_write_file_cb(void *data, size_t size, size_t nmemb, void *stream)
{
  size_t written;

  written = fwrite(data, size, nmemb, (FILE *)stream);

  return written;
}

size_t curl_write_memory_cb(void *data, size_t size, size_t nmemb, void *userdata)
{
    buffer_t *buf = (buffer_t *)userdata;
    size_t realsize = size * nmemb;

    buffer_append(buf, data, realsize);

    return realsize;
}
