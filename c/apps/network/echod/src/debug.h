#ifndef _DEBUG_H_INCLUDED_
#define _DEBUG_H_INCLUDED_ 1

#if defined(M_HAS_BUILTIN)
#undef M_HAS_BUILTIN
#endif
#if defined(__has_builtin)
#define M_HAS_BUILTIN(builtin) __has_builtin(builtin)
#else
#define M_HAS_BUILTIN(builtin) (0)
#endif

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
void __assertion_failed(char const *assertion, char const *file, int unsigned line);

# define M_ASSERT(expr) ((expr) ? (void) 0 : __assertion_failed(#expr, __FILE__, __LINE__))
#else
# defined M_ASSERT(expr) ((void) 0)
#endif

#endif  /* !defined(_DEBUG_H_INCLUDED_) */
