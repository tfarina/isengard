#ifndef USER_H_
#define USER_H_

#include "alpm_list.h"

typedef struct user_s
{
  int id;
  const char *fname;
  const char *lname;
  const char *email;
} user_t;

user_t *user_alloc(void);

/**
 * Returns 0 if the user does not exists, otherwise returns 1.
 */
int user_exists(const char* email);

int user_add(user_t *user);

int user_change(const char *fname, const char *email);

int user_delete(user_t *user);

alpm_list_t *user_get_records(void);

user_t *user_get_by_id(alpm_list_t *users, int id);

/**
 * Prints to standard output all the records found in the table.
 */
int user_print_records(void);

#endif  /* USER_H_ */
