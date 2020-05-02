#ifndef _COMMON_H_INCLUDED_
#define _COMMON_H_INCLUDED_ 1

#include "alpm_list.h"

/**
 * Prints to standard output all the contacts found in the database.
 */
int print_contact_list(alpm_list_t *contact_list);

#endif  /* !defined(_COMMON_H_INCLUDED_) */
