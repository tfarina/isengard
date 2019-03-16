#ifndef ED_INSTANCE_H_
#define ED_INSTANCE_H_

#include <unistd.h>

#define ED_INTERFACE NULL
#define ED_TCP_PORT 7
#define ED_BACKLOG 1024

typedef struct ed_options_s {
  int daemonize;      /* daemon mode */
  char *log_filename; /* log filename */

  char *interface;    /* network: listening interface */
  int port;           /* network: tcp listening port */
  int backlog;        /* network: tcp backlog */
} ed_options_t;

/** \struct instance
 * @brief An instance of Echo daemon.
 */
typedef struct ed_instance_s {
  pid_t pid;          /* process id */
  ed_options_t options;
} ed_instance_t;

void ed_instance_init(ed_instance_t *instance);

#endif  /* ED_INSTANCE_H_ */
