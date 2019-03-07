#ifndef ED_INSTANCE_H_
#define ED_INSTANCE_H_

#include <unistd.h>

#define ED_TCP_PORT 7
#define ED_BACKLOG 1024

/** \struct instance
 * @brief An instance of Echo daemon.
 */
typedef struct ed_instance_s {
  int daemonize;      /* daemon mode */
  char *log_filename; /* log filename */
  pid_t pid;          /* process id */
  int port;           /* network: tcp listening port */
  int backlog;        /* network: tcp backlog */
} ed_instance_t;

void ed_instance_init(ed_instance_t *instance);

#endif  /* ED_INSTANCE_H_ */
