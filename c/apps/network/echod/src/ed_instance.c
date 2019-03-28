#include "ed_instance.h"

#include "ed_logger.h"

static void ed_config_init(ed_config_t *config) {
  config->daemonize = 0;
  config->username = ED_USER;
  config->log_filename = NULL;
  config->pid_filename = NULL;
  config->interface = ED_INTERFACE;
  config->port = ED_TCP_PORT;
  config->backlog = ED_BACKLOG;
}

void ed_instance_init(ed_instance_t *instance) {
  instance->pid = (pid_t) -1;

  ed_config_init(&instance->config);
}
