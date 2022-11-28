#include <stdio.h>
#include <stdlib.h>

#include "ab.h"
#include "dirs.h"
#include "os_path.h"
#include "util.h"

static char const *progname;

int main(int argc, char **argv) {
  char *dbdir;
  int id;
  ab_contact_t *contact = NULL;

  progname = os_path_basename(*argv);

  if (argc != 2) {
    fprintf(stderr, "usage: %s <id>\n", progname);
    return 1;
  }

  dirs_init();

  if (!ensure_data_dir()) {
    return 1;
  }

  dbdir = dirs_get_user_data_dir();

  ab_init(dbdir);

  ab_load_contacts();

  id = atoi(argv[1]);

  contact = ab_get_contact_by_id(id);
  if (!contact) {
    fprintf(stderr, "%s: contact (%s) does not exist in our database.\n",
            progname, argv[1]);
    return 1;
  }

  if (ab_delete_contact(contact)) {
    return 1;
  }

  ab_fini();

  return 0;
}
