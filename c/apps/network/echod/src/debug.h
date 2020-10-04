/*
 * debug.h - Debugging macros for echod.
 */
#ifndef _DEBUG_H_INCLUDED_
#define _DEBUG_H_INCLUDED_ 1

#ifdef NDEBUG

#define DEBUG_PRINT(msg) ((void) 0)

#else

#define DEBUG_PRINT(msg)  \
  do {                    \
  } while (0)

#endif  /* NDEBUG */

#endif  /* !defined(_DEBUG_H_INCLUDED_) */
