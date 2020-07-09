#ifndef _ASSERTION_H_INCLUDED_
#define _ASSERTION_H_INCLUDED_ 1

#undef  M_ASSERT

#ifdef  NDEBUG

#define M_ASSERT(cond) ((void) 0)

#else

void __assertion_failed(char const *, char const *, int);

#define M_ASSERT(cond) \
  ((void) ((cond) || (__assertion_failed(#cond, __FILE__, __LINE__), 0)))

#endif  /* NDEBUG */

#endif  /* !defined(_ASSERTION_H_INCLUDED_) */
