#ifndef XASSERT_H
#define XASSERT_H

/**
 * @file xassert.h
 * @brief Assert type macro
 */

#undef  xassert

#ifdef  NDEBUG

#define xassert(cond) ((void) 0)

#else

void __assertion_failed(char const *, char const *, int);

#define xassert(cond) \
  ((void) ((cond) || (__assertion_failed(#cond, __FILE__, __LINE__), 0)))

#endif  /* NDEBUG */

#endif /* XASSERT_H */
