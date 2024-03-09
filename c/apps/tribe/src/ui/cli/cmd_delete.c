#include "commands.h"

#include <stdio.h>

#include "ab.h"
#include "dirs.h"
#include "util.h"

int cmd_delete(int argc, char **argv) {
  int rc;
  char *dbdir;
  int id;
  int deleted = 0;

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
    return 1;
  }

  id = atoi(argv[1]);

  rc = ab_delete_contact_v2(id, &deleted);
  if (rc < 0) {
    return 1;
  }

  if (!deleted) {
    fprintf(stderr, "Error: no contact with id %d was found in our database.\n",
            id);
    return 1;
  }

  ab_fini();

  return 0;
}
