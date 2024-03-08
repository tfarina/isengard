#include <stdio.h>
#include <stdlib.h>

#include "ab.h"
#include "dirs.h"
#include "os_path.h"
#include "util.h"

static char const *progname;

int main(int argc, char **argv) {
  int rc;
  char *dbdir;
  alpm_list_t *list;
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

  rc = ab_init(dbdir);
  if (rc < 0) {
    return 1;
  }

  rc = ab_load_contacts(&list);
  if (rc < 0) {
    return 1;
  }

  id = atoi(argv[1]);

  contact = ab_get_contact_by_id(id);
  if (!contact) {
    fprintf(stderr, "Error: contact (%d) does not exist in our database.\n",
            id);
    return 1;
  }

  rc = ab_delete_contact(contact);
  if (rc < 0) {
    return 1;
  }

  ab_fini();

  return 0;
}
