#include <stdio.h>
#include <stdlib.h>

#include "ab.h"
#include "dirs.h"
#include "os_path.h"
#include "util.h"

static char const *progname;

int main(int argc, char **argv) {
  int rc;
  int status = 0;
  char *dbdir;
  int id;
  ab_contact_t *contact = NULL;

  progname = os_path_basename(*argv);

  if (argc != 5) {
    fprintf(stderr, "usage: %s <id> <first-name> <last-name> <e-mail>\n", progname);
    return 1;
  }

  dirs_init();

  if (!ensure_data_dir()) {
    return 1;
  }

  dbdir = dirs_get_user_data_dir();

  rc = ab_init(dbdir);
  if (rc < 0) {
    status = 1;
    goto out;
  }

  id = atoi(argv[1]);

  rc = ab_get_contact_by_id(id, &contact);
  if (!contact) {
    fprintf(stderr, "Error: no contact with id %d was found in our database.\n",
            id);
    status = 1;
    goto out;
  }

  ab_contact_set_first_name(contact, argv[2]);
  ab_contact_set_last_name(contact, argv[3]);
  ab_contact_set_email(contact, argv[4]);

  rc = ab_update_contact(contact);
  if (rc < 0) {
    status = 1;
    goto out;
  }

out:
  if (contact)
    ab_contact_destroy(contact);

  ab_fini();

  return status;
}
