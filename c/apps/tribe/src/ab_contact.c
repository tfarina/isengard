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

int ab_contact_set_first_name(ab_contact_t *contact, char const *fname) {
  int rc = 0;
  size_t len = 0;

  len = strlen(fname) + 1;
  contact->fname = malloc(len * sizeof(char));
  if (!contact->fname) {
    rc = -1;
    goto out;
  }
  memcpy(contact->fname, fname, len);

out:
  return rc;
}

int ab_contact_set_last_name(ab_contact_t *contact, char const *lname) {
  int rc = 0;
  size_t len = 0;

  len = strlen(lname) + 1;
  contact->lname = malloc(len * sizeof(char));
  if (!contact->lname) {
    rc = -1;
    goto out;
  }
  memcpy(contact->lname, lname, len);

out:
  return rc;
}

int ab_contact_set_email(ab_contact_t *contact, char const *email) {
  int rc = 0;
  size_t len = 0;

  len = strlen(email) + 1;
  contact->email = malloc(len * sizeof(char));
  if (!contact->email) {
    rc = -1;
    goto out;
  }
  memcpy(contact->email, email, len);

out:
  return rc;
}

char *ab_contact_get_first_name(ab_contact_t *contact) {
  return contact->fname;
}

char *ab_contact_get_last_name(ab_contact_t *contact) {
  return contact->lname;
}

char *ab_contact_get_email(ab_contact_t *contact) {
  return contact->email;
}
