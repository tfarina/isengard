#ifndef MSG_H_
#define MSG_H_

/**
 * @file msg.h
 * @brief Output diagnose messages
 */

#include "attributes.h"

ATTRIBUTE_NORETURN void fatal(char const *, ...) ATTRIBUTE_PRINTFLIKE(1, 2);
void error(char const *, ...) ATTRIBUTE_PRINTFLIKE(1, 2);
void warn(char const *, ...)  ATTRIBUTE_PRINTFLIKE(1, 2);
void info(char const *, ...)  ATTRIBUTE_PRINTFLIKE(1, 2);

#endif  /* MSG_H_ */
