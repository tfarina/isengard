#ifndef CDECL_H
#define CDECL_H

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

#endif /* CDECL_H */
