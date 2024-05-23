#include "ab_contact.h"

#include <stdlib.h>
#include <string.h>

int ab_contact_create(ab_contact_t **pp_contact) {
  ab_contact_t *contact = NULL;
  int rc = 0; /* success */

  contact = malloc(sizeof(ab_contact_t));
  if (NULL == contact) {
    rc = -1;
    goto err;
  }

  memset(contact, 0, sizeof(ab_contact_t));

  contact->fname = NULL;
  contact->lname = NULL;
  contact->email = NULL;

  *pp_contact = contact;

  return 0;

err:
  free(contact);
  contact = NULL;

  return rc;
}

void ab_contact_destroy(ab_contact_t *contact) {
  if (contact == NULL) {
    return;
  }

  contact->fname = NULL;
  contact->lname = NULL;
  contact->email = NULL;

  free(contact);
  contact = NULL;
}

void ab_contact_set_first_name(ab_contact_t *contact, char const *fname) {
  size_t len = strlen(fname) + 1;
  contact->fname = malloc(len * sizeof(char));
  if (!contact->fname) {
    goto out;
  }
  memcpy(contact->fname, fname, len);

out:
  ;
}

void ab_contact_set_last_name(ab_contact_t *contact, char const *lname) {
  size_t len = strlen(lname) + 1;
  contact->lname = malloc(len * sizeof(char));
  if (!contact->lname) {
    goto out;
  }
  memcpy(contact->lname, lname, len);

out:
  ;
}

void ab_contact_set_email(ab_contact_t *contact, char const *email) {
  size_t len = strlen(email) + 1;
  contact->email = malloc(len * sizeof(char));
  if (!contact->email) {
    goto out;
  }
  memcpy(contact->email, email, len);

out:
  ;
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
