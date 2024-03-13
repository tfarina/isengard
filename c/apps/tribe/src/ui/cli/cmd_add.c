#include "commands.h"

#include <stdio.h>

#include "ab.h"
#include "dirs.h"
#include "util.h"

int cmd_add(int argc, char **argv) {
  int rc;
  char *dbdir;
  ab_contact_t *contact;

  if (argc != 4) {
    fprintf(stderr, "usage: %s <first-name> <last-name> <e-mail>\n", argv[0]);
    return 1;
  }

  dirs_init();

  if (!ensure_data_dir()) {
    return 1;
  }

  dbdir = dirs_get_user_data_dir();

  rc = ab_init(dbdir);
  if (rc < 0) {
    return 1;
  }

  contact = ab_contact_alloc();
  if (!contact) {
    return 1;
  }

  ab_contact_set_first_name(contact, argv[1]);
  ab_contact_set_last_name(contact, argv[2]);
  ab_contact_set_email(contact, argv[3]);

  rc = ab_add_contact(contact);
  if (rc < 0) {
    fputs("Failed to create a new contact.\n", stderr);
    ab_contact_free(contact);
    return 1;
  }

  ab_contact_free(contact);

  ab_fini();

  return 0;
}
