#ifndef AB_H_
#define AB_H_

#include "alpm_list.h"

typedef struct ab_contact_s
{
  int id;
  char const *fname;
  char const *lname;
  char const *email;
} ab_contact_t;

ab_contact_t *ab_contact_alloc(void);

void ab_contact_set_first_name(ab_contact_t *contact, char const *fname);
void ab_contact_set_last_name(ab_contact_t *contact, char const *lname);
void ab_contact_set_email(ab_contact_t *contact, char const *email);

char const *ab_contact_get_first_name(ab_contact_t *contact);
char const *ab_contact_get_last_name(ab_contact_t *contact);
char const *ab_contact_get_email(ab_contact_t *contact);

int ab_init(void);
int ab_close(void);

int ab_add_contact(ab_contact_t *contact);

int ab_change_contact(ab_contact_t *contact);

int ab_delete_contact(ab_contact_t *contact);

alpm_list_t *ab_get_contact_list(void);

ab_contact_t *ab_get_contact_by_id(alpm_list_t *list, int id);

/**
 * Prints to standard output all the records found in the table.
 */
int ab_print_contact_records(void);

#endif  /* AB_H_ */
