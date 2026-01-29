#ifndef B3COMPANY_H
#define B3COMPANY_H

#include "b3security.h"

typedef struct company_s {
  char *code;
  char *name;
  char *short_name;
  security_t **securities;
  unsigned nb_securities;
} company_t;

#endif /* B3COMPANY_H */
