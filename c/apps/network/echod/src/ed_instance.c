#include "ed_instance.h"

#include "ed_logger.h"

static void ed_options_init(ed_options_t *options) {
  options->daemonize = 0;
  options->log_filename = NULL;
  options->pid_filename = NULL;
  options->interface = ED_INTERFACE;
  options->port = ED_TCP_PORT;
  options->backlog = ED_BACKLOG;
}

void ed_instance_init(ed_instance_t *instance) {
  instance->pid = getpid();

  ed_logger_init();
  ed_options_init(&instance->options);
}
