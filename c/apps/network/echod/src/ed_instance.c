#include "ed_instance.h"

void ed_instance_init(ed_instance_t *instance) {
  instance->pid = (pid_t) -1;

  ed_config_init(&instance->config);
}
