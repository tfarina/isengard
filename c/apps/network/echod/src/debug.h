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

#ifndef NDEBUG
void __assert_fail( char const *assertion, char const *file, int unsigned line, char const *func);

# define M_ASSERT(expr) ((expr) ? (void) 0 : __assert_fail(#expr,__FILE__,__LINE__,__func__))
#else
# defined M_ASSERT(expr) ((void) 0)
#endif


#endif  /* !defined(_DEBUG_H_INCLUDED_) */
