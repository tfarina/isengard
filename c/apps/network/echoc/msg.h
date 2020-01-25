#ifndef MSG_H_
#define MSG_H_

#include "hints.h"

void fatal(char const *, ...) ATTR_FORMAT_PRINTF(1, 2) ATTR_NORETURN;
void error(char const *, ...) ATTR_FORMAT_PRINTF(1, 2);
void warn(char const *, ...) ATTR_FORMAT_PRINTF(1, 2);
void info(char const *, ...) ATTR_FORMAT_PRINTF(1, 2);

#endif  /* MSG_H_ */
