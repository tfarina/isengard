#ifndef _CDECL_H_INCLUDED_
#define _CDECL_H_INCLUDED_ 1

/**
 * This informs C++ that the types and declarations are C, not C++.
 */
#ifdef __cplusplus
# define BEGIN_C_DECLS extern "C" {
# define END_C_DECLS }
#else
# define BEGIN_C_DECLS
# define END_C_DECLS
#endif

#endif  /* !defined(_CDECL_H_INCLUDED_) */
