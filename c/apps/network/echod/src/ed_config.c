#include "ed_config.h"

#include <stdlib.h>

void ed_config_init(ed_config_t *config) {
  config->pid = (pid_t) -1;
  config->daemonize = 0;
  config->conffile = NULL;
  config->logfile = NULL;
  config->pidfile = ED_DEFAULT_PIDFILE;
  config->username = ED_USER;
  config->interface = ED_DEFAULT_LISTEN_ADDR;
  config->port = ED_DEFAULT_LISTEN_PORT;
  config->backlog = ED_DEFAULT_LISTEN_BACKLOG;
}
