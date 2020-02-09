#ifndef HINTS_H_
#define HINTS_H_

#if defined(MK_NORETURN)
#undef MK_NORETURN
#endif
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define MK_NORETURN __attribute__((__noreturn__))
#elif defined(_MSC_VER)
#define MK_NORETURN __declspec(noreturn)
#else
#define MK_NORETURN
#endif

#if defined(MK_PRINTF)
#undef MK_PRINTF
#endif
#if defined(__GNUC__)
#define MK_PRINTF(string_index, first_to_check) __attribute__ ((__format__ (__printf__, string_index, first_to_check)))
#else
#define MK_PRINTF(string_index, first_to_check)
#endif

#endif  /* HINTS_H_ */
