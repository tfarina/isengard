#ifndef _MKDIRP_H_INCLUDED_
#define _MKDIRP_H_INCLUDED_ 1

#include <sys/types.h>
#include <sys/stat.h>

int f_mkdirp(char const *pathname, mode_t mode);

#endif  /* !defined(_MKDIRP_H_INCLUDED_) */
