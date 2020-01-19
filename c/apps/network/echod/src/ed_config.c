#include "ed_config.h"

#include "ed_default_paths.h"

/* User for running the Echo daemon. */
#define ED_USER "_echod"

#define ED_DEFAULT_BIND_ADDR "0.0.0.0"
#define ED_DEFAULT_ECHO_PORT 7
#define ED_DEFAULT_BACKLOG 1024

void ed_config_init(ed_config_t *config) {
  config->detach = 1;
  config->conffile = DEF_CONFIG_FILE_PATH;
  config->pidfile = DEF_PID_FILE_PATH;
  config->user = ED_USER;
  config->address = ED_DEFAULT_BIND_ADDR;
  config->port = ED_DEFAULT_ECHO_PORT;
  config->backlog = ED_DEFAULT_BACKLOG;
  config->logfile = (char *) 0;
}
