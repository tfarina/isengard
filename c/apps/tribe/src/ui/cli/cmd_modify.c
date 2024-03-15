#include "commands.h"

#include <stdio.h>

#include "ab.h"
#include "dirs.h"
#include "os_path.h"
#include "util.h"

int cmd_modify(int argc, char **argv) {
  int rc;
  char *dbdir;
  int id;
  ab_contact_t *contact = NULL;

  if (argc != 5) {
    fprintf(stderr, "usage: %s <id> <first-name> <last-name> <e-mail>\n", argv[0]);
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

  id = atoi(argv[1]);

  rc = ab_get_contact_by_id(id, &contact);
  if (!contact) {
    fprintf(stderr, "Error: no contact with id %d was found in our database.\n",
            id);
    return 1;
  }

  ab_contact_set_first_name(contact, argv[2]);
  ab_contact_set_last_name(contact, argv[3]);
  ab_contact_set_email(contact, argv[4]);

  rc = ab_update_contact(contact);
  if (rc < 0) {
    return 1;
  }

  ab_fini();

  return 0;
}
