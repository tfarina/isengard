#ifndef USER_H_
#define USER_H_

#include "alpm_list.h"

typedef struct user_s
{
  const char *fname;
  const char *lname;
  const char *email;
} user_t;

user_t *user_alloc(void);

/**
 * Returns 0 if the user does not exists, otherwise returns 1.
 */
int user_exists(const char* email);

int user_add(const char* fname,
             const char* lname,
             const char* email);

int user_change(const char *fname, const char *email);

int user_delete(const char *email);

alpm_list_t *user_get_records(void);

/**
 * Prints to standard output all the records found in the table.
 */
int user_print_records(void);

#endif  /* USER_H_ */
