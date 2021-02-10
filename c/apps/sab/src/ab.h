#ifndef AB_H_
#define AB_H_

#include "ab_contact.h"
#include "alpm_list.h"

int ab_init(char *dbpath);
int ab_fini(void);

void ab_load_contacts(void);

int ab_add_contact(ab_contact_t *contact);

int ab_change_contact(ab_contact_t *contact);

int ab_delete_contact(ab_contact_t *contact);

alpm_list_t *ab_get_contact_list(void);

ab_contact_t *ab_get_contact_by_id(int id);

#endif  /* AB_H_ */
