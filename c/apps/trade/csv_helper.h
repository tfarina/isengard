#ifndef CSV_HELPER_H_
#define CSV_HELPER_H_

#include <stddef.h>

void process_field(void *field, size_t field_len __attribute__((unused)), void *ctx);
void process_row(int delim __attribute__((unused)), void *ctx);

#endif  /* CSV_HELPER_H_ */
