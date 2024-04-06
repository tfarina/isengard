#include "ab_contact.h"

#include <stdlib.h>

int ab_contact_create(ab_contact_t **pp_contact) {
  ab_contact_t *contact = NULL;

  contact = malloc(sizeof(ab_contact_t));
  if (NULL == contact) {
    return -1;
  }

  contact->fname = NULL;
  contact->lname = NULL;
  contact->email = NULL;

  *pp_contact = contact;

  return 0;
}

void ab_contact_destroy(ab_contact_t *contact) {
  if (contact == NULL) {
    return;
  }

  contact->fname = NULL;
  contact->lname = NULL;
  contact->email = NULL;

  free(contact);
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
