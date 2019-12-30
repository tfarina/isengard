#ifndef CDECL_H_
#define CDECL_H_

#ifdef __cplusplus
/** If using C++ this macro enables C mode, otherwise does nothing. */
#define C_DECL_BEGIN extern "C" {
/** If using C++ this macro switches back to C++ mode, otherwise does nothing. */
#define C_DECL_END }

#else

#define C_DECL_BEGIN
#define C_DECL_END

#endif 

#endif  /* CDECL_H_ */
