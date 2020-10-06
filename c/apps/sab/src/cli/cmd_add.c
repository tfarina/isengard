#include "commands.h"

#include <stdio.h>

#include "ab.h"

int cmd_add(int argc, char **argv) {
  ab_contact_t *contact;

  if (argc != 4) {
    printf("usage: add 'FIRST NAME' 'LAST NAME' 'E-MAIL'\n");
    return -1;
  }

  ab_init();

  contact = ab_contact_alloc();
  if (!contact) {
    return -1;
  }

  ab_contact_set_first_name(contact, argv[1]);
  ab_contact_set_last_name(contact, argv[2]);
  ab_contact_set_email(contact, argv[3]);

  ab_add_contact(contact);

  ab_contact_free(contact);

  ab_fini();

  return 0;
}
