#ifndef MSG_H_
#define MSG_H_

#include "attributes.h"

MK_NORETURN void fatal(char const *, ...) MK_PRINTFLIKE(1, 2);
void error(char const *, ...) MK_PRINTFLIKE(1, 2);
void warn(char const *, ...)  MK_PRINTFLIKE(1, 2);
void info(char const *, ...)  MK_PRINTFLIKE(1, 2);

#endif  /* MSG_H_ */
