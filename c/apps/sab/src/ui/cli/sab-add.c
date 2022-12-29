#include <stdio.h>
#include <stdlib.h>

#include "ab.h"
#include "dirs.h"
#include "os_path.h"
#include "util.h"

static char const *progname;

int main(int argc, char **argv) {
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

  ab_init(dbdir);

  contact = ab_contact_alloc();
  if (!contact) {
    return 1;
  }

  ab_contact_set_first_name(contact, argv[1]);
  ab_contact_set_last_name(contact, argv[2]);
  ab_contact_set_email(contact, argv[3]);

  /* It's okay to add duplicate records. */
  if (ab_add_contact(contact)) {
    return 1;
  }

  ab_fini();

  return 0;
}
