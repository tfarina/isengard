#include <stdio.h>
#include <stdlib.h>

#include "ab.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

int main(int argc, char **argv) {
  ab_contact_t *contact = NULL;

  if (argc != 4) {
    printf("usage: %s 'FIRST NAME' 'LAST NAME' 'E-MAIL'\n", argv[0]);
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

  /* It's okay to add duplicate records. */
  if (ab_add_contact(contact)) {
    return -1;
  }

  ab_fini();

  return 0;
}
