#include "commands.h"

#include <stdio.h>

#include "ab.h"
#include "dirs.h"
#include "util.h"

int cmd_add(int argc, char **argv) {
  char *dbdir;
  ab_contact_t *contact;

  if (argc != 4) {
    printf("usage: add 'FIRST NAME' 'LAST NAME' 'E-MAIL'\n");
    return 1;
  }

  dirs_init();

  if (!ensure_data_dir()) {
    return 1;
  }

  dbdir = dirs_get_user_data_dir();

  ab_init(dbdir);

  contact = ab_contact_alloc();
  if (!contact) {
    return 1;
  }

  ab_contact_set_first_name(contact, argv[1]);
  ab_contact_set_last_name(contact, argv[2]);
  ab_contact_set_email(contact, argv[3]);

  ab_add_contact(contact);

  ab_contact_free(contact);

  ab_fini();

  return 0;
}
