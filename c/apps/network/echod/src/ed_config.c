#include "ed_config.h"

#include "ed_default_paths.h"

/* User for running the Echo daemon. */
#define DEF_DAEMON_USER "_echod"

#define DEF_BIND_ADDR "0.0.0.0"
#define DEF_ECHO_PORT 7
#define DEF_BACKLOG   1024

void ed_config_init(ed_config_t *config) {
  config->detach = 1;
  config->conffile = DEF_PATH_ECHODCONF;
  config->pidfile = DEF_PATH_ECHODPID;
  config->user = DEF_DAEMON_USER;
  config->address = DEF_BIND_ADDR;
  config->port = DEF_ECHO_PORT;
  config->backlog = DEF_BACKLOG;
  config->logfile = (char *) 0;
}
