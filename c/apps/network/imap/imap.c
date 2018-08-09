#include <string.h>
#include <stdlib.h>

#include <curl/curl.h>

struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t write_data_to_memory(void *ptr, size_t size, size_t nmemb, struct string *s) {
  size_t realsize = size * nmemb;
  size_t new_len = s->len + realsize;
  s->ptr = realloc(s->ptr, new_len + 1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, realsize);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return realsize;
}

/* http://tiku.io/questions/1891989/c-use-libcurl-to-list-imap-sent-mails */
int main(int argc, char *argv[]) {
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if (curl) {
    struct string s;
    init_string(&s);
    curl_easy_setopt(curl, CURLOPT_USERNAME, argv[1]);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, argv[2]);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

    curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/\"[Gmail]/All Mail\"");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_memory);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);

    printf("%s\n", s.ptr);
    free(s.ptr);

    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }
  return 0;
}
