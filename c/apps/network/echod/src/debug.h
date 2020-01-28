#ifndef _DEBUG_H_INCLUDED_
#define _DEBUG_H_INCLUDED_ 1

/*
 * Branch prediction hints.
 */
#if __GNUC__ >= 3
# define M_UNLIKELY(exp) __builtin_expect ((exp), 0)
# define M_LIKELY(exp)   __builtin_expect ((exp), 1)
#else
# define M_UNLIKELY(exp) (exp)
# define M_LIKELY(exp)   (exp)
#endif

#endif  /* !defined(_DEBUG_H_INCLUDED_) */
