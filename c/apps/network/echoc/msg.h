#ifndef MSG_H_
#define MSG_H_

#include "hints.h"

void fatal(const char *fmt, ...) ATTR_FORMAT_PRINTF(1, 2) ATTR_NORETURN;
void error(const char *fmt, ...) ATTR_FORMAT_PRINTF(1, 2);
void warn(const char *fmt, ...) ATTR_FORMAT_PRINTF(1, 2);
void info(const char *fmt, ...) ATTR_FORMAT_PRINTF(1, 2);

#endif  /* MSG_H_ */
