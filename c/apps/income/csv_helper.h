#ifndef CSV_HELPER_H_
#define CSV_HELPER_H_

#include <stddef.h>

#define UNUSED __attribute__((unused))

void process_field(void *field, size_t field_len UNUSED, void *ctx);
void process_row(int delim UNUSED, void *ctx);

#endif  /* CSV_HELPER_H_ */
