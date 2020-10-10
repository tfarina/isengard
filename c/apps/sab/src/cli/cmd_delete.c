#include "commands.h"

#include <stdio.h>

#include "ab.h"
#include "util.h"

int cmd_delete(int argc, char **argv) {
  char *dbdir;
  int id;
  ab_contact_t *contact = NULL;

  if (argc != 2) {
    fprintf(stderr, "usage: %s 'ID'\n", argv[0]);
    return 1;
  }

  dbdir = ensure_data_dir();

  ab_init(dbdir);

  ab_load_contacts();

  id = atoi(argv[1]);

  contact = ab_get_contact_by_id(id);
  if (!contact) {
    fprintf(stderr, "%s: contact (%s) does not exist in our database.\n",
            argv[0], argv[1]);
    return 1;
  }

  if (ab_delete_contact(contact)) {
    return 1;
  }

  ab_fini();

  return 0;
}
