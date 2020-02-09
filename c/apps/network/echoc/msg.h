#ifndef MSG_H_
#define MSG_H_

#include "hints.h"

void fatal(char const *, ...) MK_PRINTF(1, 2) MK_NORETURN;
void error(char const *, ...) MK_PRINTF(1, 2);
void warn(char const *, ...)  MK_PRINTF(1, 2);
void info(char const *, ...)  MK_PRINTF(1, 2);

#endif  /* MSG_H_ */
