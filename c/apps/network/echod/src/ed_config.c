#include "ed_config.h"

#include <stdlib.h>

#include "ed_default_paths.h"

void ed_config_init(ed_config_t *config) {
  config->detach = 1;
  config->conffile = DEF_CONFIG_FILE_PATH;
  config->pidfile = DEF_PID_FILE_PATH;
  config->user = ED_USER;
  config->address = ED_DEFAULT_BIND_ADDR;
  config->port = ED_DEFAULT_ECHO_PORT;
  config->backlog = ED_DEFAULT_BACKLOG;
  config->logfile = NULL;
}
