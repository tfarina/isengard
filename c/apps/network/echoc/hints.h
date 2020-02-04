#ifndef HINTS_H_
#define HINTS_H_

#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define ATTR_NORETURN __attribute__((__noreturn__))
#elif defined(_MSC_VER)
#define ATTR_NORETURN __declspec(noreturn)
#else
#define ATTR_NORETURN
#endif

#if defined(ATTR_FORMAT_PRINTF)
#undef ATTR_FORMAT_PRINTF
#endif
#if defined(__GNUC__)
#define ATTR_FORMAT_PRINTF(string_index, first_to_check) __attribute__ ((__format__ (__printf__, string_index, first_to_check)))
#else
#define ATTR_FORMAT_PRINTF(string_index, first_to_check)
#endif

#endif  /* HINTS_H_ */
