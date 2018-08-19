#include <string.h>
#include <stdlib.h>

#include <curl/curl.h>

typedef struct {
  char *data;
  size_t length;
} buffer_t;

void buffer_init(buffer_t *b) {
  b->length = 0;
  b->data = malloc(b->length + 1);
  if (b->data == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  b->data[0] = '\0';
}

size_t write_data_to_memory(void *data, size_t size, size_t nmemb, buffer_t *b) {
  size_t realsize = size * nmemb;
  size_t new_len = b->length + realsize;

  b->data = realloc(b->data, new_len + 1);
  if (b->data == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(b->data + b->length, data, realsize);
  b->data[new_len] = '\0';
  b->length = new_len;

  return realsize;
}

/* http://tiku.io/questions/1891989/c-use-libcurl-to-list-imap-sent-mails */
int main(int argc, char *argv[]) {
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if (curl) {
    buffer_t buf;
    buffer_init(&buf);

    curl_easy_setopt(curl, CURLOPT_USERNAME, argv[1]);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, argv[2]);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

    curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/\"[Gmail]/All Mail\"");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_memory);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
    res = curl_easy_perform(curl);

    printf("%s\n", buf.data);
    free(buf.data);

    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }

  return 0;
}
