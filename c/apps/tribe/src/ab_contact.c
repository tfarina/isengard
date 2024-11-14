#include "ab_contact.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

int ab_contact_create(ab_contact_t **pp_contact) {
  ab_contact_t *contact = NULL;
  int rc = 0; /* success */

  /* Allocate space for the ab_contact structure
   */
  contact = malloc(sizeof(ab_contact_t));
  if (!contact) {
    rc = -ENOMEM;
    goto err;
  }

  /* Zero init the object
   */
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
  if (!contact) {
    return;
  }

  free(contact->fname);
  contact->fname = NULL;
  free(contact->lname);
  contact->lname = NULL;
  free(contact->email);
  contact->email = NULL;

  free(contact);
  contact = NULL;
}

int ab_contact_set_first_name(ab_contact_t *contact, char const *fname) {
  int rc = 0;
  size_t len = 0;

  free(contact->fname);

  len = strlen(fname) + 1;
  contact->fname = malloc(len * sizeof(char));
  if (!contact->fname) {
    rc = -ENOMEM;
    goto out;
  }
  memcpy(contact->fname, fname, len);

out:
  return rc;
}

int ab_contact_set_last_name(ab_contact_t *contact, char const *lname) {
  int rc = 0;
  size_t len = 0;

  free(contact->lname);

  len = strlen(lname) + 1;
  contact->lname = malloc(len * sizeof(char));
  if (!contact->lname) {
    rc = -ENOMEM;
    goto out;
  }
  memcpy(contact->lname, lname, len);

out:
  return rc;
}

int ab_contact_set_email(ab_contact_t *contact, char const *email) {
  int rc = 0;
  size_t len = 0;

  free(contact->email);

  len = strlen(email) + 1;
  contact->email = malloc(len * sizeof(char));
  if (!contact->email) {
    rc = -ENOMEM;
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
