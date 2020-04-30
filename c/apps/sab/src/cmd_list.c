#include "commands.h"

#include "ab.h"
#include "common.h"

int cmd_list(int argc, char **argv) {
  int rc;

  ab_init();

  rc = print_contact_list(ab_get_contact_list());

  ab_close();

  return rc;
}
