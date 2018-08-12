#ifndef LOG_H_
#define LOG_H_

#include "macros.h"

NORETURN void fatal(const char *msg, ...) ATTRIBUTE_PRINTF(1, 2);

#endif  /* LOG_H_ */
