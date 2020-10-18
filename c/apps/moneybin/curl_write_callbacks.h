#ifndef CURL_WRITE_CALLBACKS_H_
#define CURL_WRITE_CALLBACKS_H_

#include <stddef.h>

size_t curl_write_file_cb(void *data, size_t size, size_t nmemb, void *stream);

size_t curl_write_memory_cb(void *data, size_t size, size_t nmemb, void *userdata);

#endif /* CURL_WRITE_CALLBACKS_H_ */
