#include "commands.h"

#include <stdio.h>

#include "ab.h"
#include "dirs.h"
#include "util.h"

int cmd_delete(int argc, char **argv) {
  int rc;
  int status = 0;
  char *dbdir;
  int id;
  ab_contact_t *contact = NULL;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <id>\n", argv[0]);
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
  if (rc < 0 || !contact) {
    fprintf(stderr, "Error: no contact with id %d was found in our database.\n",
            id);
    status = 1;
    goto out;
  }

  rc = ab_delete_contact_v2(id);
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
