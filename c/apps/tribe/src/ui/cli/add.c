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
  ab_contact_t *contact = NULL;

  progname = os_path_basename(*argv);

  if (argc != 4) {
    fprintf(stderr, "usage: %s <first-name> <last-name> <e-mail>\n", progname);
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
    goto done;
  }

  rc = ab_contact_create(&contact);
  if (rc < 0 || !contact) {
    status = 1;
    goto done;
  }

  ab_contact_set_first_name(contact, argv[1]);
  ab_contact_set_last_name(contact, argv[2]);
  ab_contact_set_email(contact, argv[3]);

  /* It's okay to add duplicate records. */
  rc = ab_add_contact_v2(contact);
  if (rc < 0) {
    fputs("Failed to create a new contact.\n", stderr);
    status = 1;
    goto done;
  }

done:
  if (contact)
    ab_contact_destroy(contact);

  ab_fini();

  return status;
}