#ifndef MACROS_H_
#define MACROS_H_

#if defined(__GNUC__)
#define NORETURN __attribute__((__noreturn__))
#elif defined(_MSC_VER)
#define NORETURN __declspec(noreturn)
#else
#define NORETURN
#endif

#ifndef ATTRIBUTE_PRINTF
#if defined(__GNUC__)
#define ATTRIBUTE_PRINTF(m, n) __attribute__ ((__format__ (__printf__, m, n)))
#else
#define ATTRIBUTE_PRINTF(m, n)
#endif
#endif

#endif  /* MACROS_H_ */
