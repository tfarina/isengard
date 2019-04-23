#include "ed_config.h"

#include <stdlib.h>

void ed_config_init(ed_config_t *config) {
  config->daemonize = 0;
  config->username = ED_USER;
  config->conf_filename = NULL;
  config->log_filename = NULL;
  config->pid_filename = NULL;
  config->interface = ED_DEFAULT_LISTEN_ADDR;
  config->port = ED_DEFAULT_LISTEN_PORT;
  config->backlog = ED_DEFAULT_LISTEN_BACKLOG;
}
