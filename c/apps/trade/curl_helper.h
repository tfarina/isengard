#ifndef CURL_HELPER_H_
#define CURL_HELPER_H_

#include <stddef.h>

size_t write_data_to_file(void *data, size_t size, size_t nmemb, void *stream);

size_t write_data_to_memory(void *data, size_t size, size_t nmemb, void *ctx);

#endif /* CURL_HELPER_H_ */
