#include "abook_contact.h"

#include <stdlib.h>

ab_contact_t *ab_contact_alloc(void) {
  ab_contact_t *contact;

  contact = (ab_contact_t *) malloc(sizeof(*contact));
  if (contact == NULL) {
    return NULL;
  }

  contact->fname = NULL;
  contact->lname = NULL;
  contact->email = NULL;

  return contact;
}

void ab_contact_set_first_name(ab_contact_t *contact, char const *fname) {
  contact->fname = fname;
}

void ab_contact_set_last_name(ab_contact_t *contact, char const *lname) {
  contact->lname = lname;
}

void ab_contact_set_email(ab_contact_t *contact, char const *email) {
  contact->email = email;
}

char const *ab_contact_get_first_name(ab_contact_t *contact) {
  return contact->fname;
}

char const *ab_contact_get_last_name(ab_contact_t *contact) {
  return contact->lname;
}

char const *ab_contact_get_email(ab_contact_t *contact) {
  return contact->email;
}
