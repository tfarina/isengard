#ifndef CDECL_H_
#define CDECL_H_

#ifdef __cplusplus
/** If using C++ this macro enables C mode, otherwise does nothing. */
#define BEGIN_C_DECLS extern "C" {
/** If using C++ this macro switches back to C++ mode, otherwise does nothing. */
#define END_C_DECLS }

#else

#define BEGIN_C_DECLS
#define END_C_DECLS

#endif 

#endif  /* CDECL_H_ */
