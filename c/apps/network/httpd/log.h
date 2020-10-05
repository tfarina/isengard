#ifndef LOG_H_
#define LOG_H_

#include "attributes.h"

MK_NORETURN void fatal(const char *msg, ...) MK_PRINTF(1, 2);

#endif  /* LOG_H_ */
