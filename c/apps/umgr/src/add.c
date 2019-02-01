#include <stdio.h>
#include <stdlib.h>

#include "user.h"

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm

int main(int argc, char **argv) {
  ab_contact_t *contact = NULL;

  if (argc != 4) {
    printf("usage: %s 'FIRST NAME' 'LAST NAME' 'E-MAIL'\n", argv[0]);
    return -1;
  }

  ab_init();

  contact = ab_contact_alloc();
  contact->fname = argv[1];
  contact->lname = argv[2];
  contact->email = argv[3];

  /* It's okay to add duplicate records. */
  if (ab_add_contact(contact)) {
    return -1;
  }

  ab_close();

  return 0;
}
