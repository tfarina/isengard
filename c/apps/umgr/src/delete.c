#include <stdio.h>
#include <stdlib.h>

#include "ab.h"

int main(int argc, char **argv) {
  alpm_list_t *list;
  int id;
  ab_contact_t *contact = NULL;

  if (argc != 2) {
    fprintf(stderr, "usage: %s 'ID'\n", argv[0]);
    return -1;
  }

  ab_init();

  list = ab_get_contact_list();

  id = atoi(argv[1]);

  contact = ab_get_contact_by_id(list, id);
  if (!contact) {
    fprintf(stderr, "%s: contact (%s) does not exist in our database.\n",
            argv[0], argv[1]);
    return -1;
  }

  if (ab_delete_contact(contact)) {
    return -1;
  }

  ab_close();

  return 0;
}
