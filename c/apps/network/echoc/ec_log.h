#ifndef LOG_H_
#define LOG_H_

#include "ec_macros.h"

void fatal(const char *fmt, ...) EC_FORMAT_PRINTF(1, 2) EC_NORETURN;
void error(const char *fmt, ...) EC_FORMAT_PRINTF(1, 2);
void warning(const char *fmt, ...) EC_FORMAT_PRINTF(1, 2);
void info(const char *fmt, ...) EC_FORMAT_PRINTF(1, 2);

#endif  /* LOG_H_ */
