#ifndef EC_MACROS_H_
#define EC_MACROS_H_

#if defined(__GNUC__)
#define EC_NORETURN __attribute__((__noreturn__))
#elif defined(_MSC_VER)
#define EC_NORETURN __declspec(noreturn)
#else
#define EC_NORETURN
#endif

#ifndef EC_FORMAT_PRINTF
#define EC_FORMAT_PRINTF(m, n) __attribute__ ((__format__ (__printf__, m, n)))
#endif

#endif  /* EC_MACROS_H_ */
