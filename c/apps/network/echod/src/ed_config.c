#include "ed_config.h"

#include <stdlib.h>

void ed_config_init(ed_config_t *config) {
  config->daemonize = 0;
  config->username = ED_USER;
  config->log_filename = NULL;
  config->pid_filename = NULL;
  config->interface = ED_INTERFACE;
  config->port = ED_TCP_PORT;
  config->backlog = ED_BACKLOG;
}
