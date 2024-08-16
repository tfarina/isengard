#ifndef AB_H_
#define AB_H_

#include "ab_contact.h"
#include "alpm_list.h"

int ab_init(char *dbpath);
int ab_fini(void);

int ab_enum_contacts(alpm_list_t **pp_contact_list);
int ab_enum_contacts_v2(int *p_count, ab_contact_t **pp_contacts);

int ab_add_contact(ab_contact_t *contact);
int ab_add_contact_v2(ab_contact_t *contact);

int ab_update_contact(ab_contact_t *contact);

int ab_delete_contact(ab_contact_t *contact);
int ab_delete_contact_v2(int id);

int ab_get_contact_by_id(int id, ab_contact_t **pp_contact);

#endif  /* AB_H_ */
