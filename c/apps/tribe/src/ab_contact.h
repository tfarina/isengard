#ifndef AB_CONTACT_H
#define AB_CONTACT_H

typedef struct ab_contact_s
{
  int id;
  char *fname;
  char *lname;
  char *email;
} ab_contact_t;

int ab_contact_create(ab_contact_t **pp_contact);
void ab_contact_destroy(ab_contact_t *contact);

int ab_contact_set_first_name(ab_contact_t *contact, char const *fname);
int ab_contact_set_last_name(ab_contact_t *contact, char const *lname);
int ab_contact_set_email(ab_contact_t *contact, char const *email);

char *ab_contact_get_first_name(ab_contact_t *contact);
char *ab_contact_get_last_name(ab_contact_t *contact);
char *ab_contact_get_email(ab_contact_t *contact);

#endif /* AB_CONTACT_H */
