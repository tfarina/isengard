#ifndef DIE_H_
#define DIE_H_

#if defined(__GNUC__)
#define NORETURN __attribute__((__noreturn__))
#elif defined(_MSC_VER)
#define NORETURN __declspec(noreturn)
#else
#define NORETURN
#endif

NORETURN void die(const char *msg, ...) __attribute__((format(printf, 1, 2)));

#endif  /* DIE_H_ */
