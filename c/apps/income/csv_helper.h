#ifndef CSV_HELPER_H_
#define CSV_HELPER_H_

#include <stddef.h>

#define UNUSED __attribute__((unused))

void csv_column_cb(void *field, size_t field_len UNUSED, void *ctx);
void csv_row_cb(int delim UNUSED, void *ctx);

#endif  /* CSV_HELPER_H_ */
