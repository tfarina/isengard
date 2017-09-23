#ifndef LOG_H_
#define LOG_H_

#if defined(__GNUC__)
#define NORETURN __attribute__((__noreturn__))
#elif defined(_MSC_VER)
#define NORETURN __declspec(noreturn)
#else
#define NORETURN
#endif

#ifndef ATTRIBUTE_PRINTF
#define ATTRIBUTE_PRINTF(m, n) __attribute__ ((__format__ (__printf__, m, n)))
#endif

NORETURN void fatal(const char *fmt, ...) ATTRIBUTE_PRINTF(1, 2);
void error(const char *fmt, ...) ATTRIBUTE_PRINTF(1, 2);
void warning(const char *fmt, ...) ATTRIBUTE_PRINTF(1, 2);
void info(const char *fmt, ...) ATTRIBUTE_PRINTF(1, 2);

#endif  /* LOG_H_ */
