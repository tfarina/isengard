#ifndef _B3COMPANY_H_INCLUDED_
#define _B3COMPANY_H_INCLUDED_ 1

#include "b3security.h"

typedef struct company_s {
  char *code;
  char *name;
  char *short_name;
  security_t **securities;
  unsigned nb_securities;
} company_t;

#endif  /* !defined(_B3COMPANY_H_INCLUDED_) */
