#include "ed_instance.h"

void ed_instance_init(ed_instance_t *instance) {
  instance->pid = getpid();

  instance->options.daemonize = 0;
  instance->options.log_filename = NULL;
  instance->options.interface = ED_INTERFACE;
  instance->options.port = ED_TCP_PORT;
  instance->options.backlog = ED_BACKLOG;
}
