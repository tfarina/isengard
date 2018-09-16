#ifndef ED_LOG_H_
#define ED_LOG_H_

#include <stdarg.h>

void log_init(const char *progname, int on_stderr);
void vlog(int pri, const char *fmt, va_list ap);
void log_info(const char *emsg, ...);
void log_error(const char *emsg, ...);

#endif  /* ED_LOG_H_ */
