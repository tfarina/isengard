#include "ed_config.h"

#include <stdlib.h>

#include "ed_default_paths.h"

void ed_config_init(ed_config_t *config) {
  config->daemonize = 0;
  config->conffile = ED_CONFIG_FILE_PATH;
  config->logfile = NULL;
  config->pidfile = ED_PID_FILE_PATH;
  config->username = ED_USER;
  config->address = ED_DEFAULT_LISTEN_ADDR;
  config->port = ED_DEFAULT_LISTEN_PORT;
  config->backlog = ED_DEFAULT_LISTEN_BACKLOG;
}

int ed_config_load_file(ed_config_t *config) {
  return 0;
}
