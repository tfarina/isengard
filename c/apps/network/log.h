#ifndef LOG_H_
#define LOG_H_

#include "attributes.h"

MK_NORETURN void fatal(const char *fmt, ...) MK_PRINTF(1, 2);
void error(const char *fmt, ...) MK_PRINTF(1, 2);
void warning(const char *fmt, ...) MK_PRINTF(1, 2);
void info(const char *fmt, ...) MK_PRINTF(1, 2);

#endif  /* LOG_H_ */
