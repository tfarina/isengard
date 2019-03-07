#include "ed_instance.h"

void ed_instance_init(ed_instance_t *instance) {
  instance->daemonize = 0;
  instance->log_filename = NULL;
  instance->pid = getpid();
  instance->port = ED_TCP_PORT;
  instance->backlog = ED_BACKLOG;
}
