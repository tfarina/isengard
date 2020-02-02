#include "option.h"

#include "ed_default_paths.h"

/* User for running the Echo daemon. */
#define DEF_DAEMON_USER "_echod"

#define DEF_BIND_ADDR "0.0.0.0"
#define DEF_ECHO_PORT 7
#define DEF_BACKLOG   1024

void option_init(option_t *opt) {
  opt->detach = 1;
  opt->conffile = DEF_PATH_ECHODCONF;
  opt->pidfile = DEF_PATH_ECHODPID;
  opt->user = DEF_DAEMON_USER;
  opt->address = DEF_BIND_ADDR;
  opt->port = DEF_ECHO_PORT;
  opt->backlog = DEF_BACKLOG;
  opt->logfile = (char *) 0;
}
